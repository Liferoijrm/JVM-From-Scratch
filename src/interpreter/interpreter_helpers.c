#include "interpreter_helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static u1 LinkAllLoaded(MethodArea *method_area);

ClassFile* get_class_from_constant_pool(JVMThread* thread, MethodArea* method_area, Frame* frame, char* class_name) {

    // 1. Tenta buscar na MethodArea
    MethodAreaEntry* entry = MethodAreaGetEntry(method_area, class_name);

    // 2. Se não estiver carregada, faz Load e Link
    if (entry == NULL) {
        printf("[RESOLVE] Carregando a classe '%s' dinamicamente...\n", class_name);
        
        ClassFile* loaded_cf = LoadClass(method_area, class_name);
        if (loaded_cf == NULL) {
            printf("NoClassDefFoundError: %s\n", class_name);
            exit(1);
        }

        entry = MethodAreaGetEntry(method_area, class_name);
        
        if (!LinkAllLoaded(method_area)) {
            printf("LinkageError: falha ao linkar classes dinamicamente\n", class_name);
            exit(1);
        }
    }

    // 3. Verifica se precisa de inicialização (<clinit>)
    // Assumindo que sua MethodAreaEntry tenha um campo 'state' (ex: CLASS_LOADED, CLASS_INITIALIZED)
    if (entry->state < CLASS_INITIALIZING) {
        printf("[RESOLVE] Inicializando a classe '%s'...\n", class_name);
        
        // Esta função deve procurar o método <clinit>. 
        // Se achar, cria o Frame e faz push(thread->frame_stack, clinit_frame).
        // Também deve mudar o entry->state para CLASS_INITIALIZED para não repetir o processo.
        u1 has_clinit = InitializeClass(method_area, entry, thread);
        
        if (has_clinit) {
            // Retornamos NULL como "Sinal" de que a thread foi pausada para rodar o clinit.
            return NULL;
        }
    }

    // Se já estava inicializada (ou se não tinha <clinit>), retorna a classe pronta!
    return entry->class_file;
}

static u1 LinkAllLoaded(MethodArea *method_area){
    for (u2 i = 0; i < method_area->count; i++) {
        MethodAreaEntry *entry = &method_area->entries[i];
        if (entry->state == CLASS_LOADED){
            if (!LinkClass(entry, method_area)){
                fprintf(stderr, "Falha ao linkar '%s'\n",
                        entry->class_name ? entry->class_name : "?");
                return 0;
            }
        }
    }
    return 1;
}

static void print_java_float(float f) {
    if (f == (long long)f && fabsf(f) < 1e7f) printf("%.1f", f);
    else printf("%.7g", f);
}

static void print_java_double(double d) {
    if (d == (long long)d && fabs(d) < 1e15) printf("%.1f", d);
    else printf("%.16g", d);
}

u1 is_native_class(const char *name, u2 len) {
    return (len == 16 && strncmp(name, "java/lang/String", 16) == 0) ||
           (len == 22 && strncmp(name, "java/lang/StringBuffer", 22) == 0) ||
           (len == 16 && strncmp(name, "java/lang/System", 16) == 0) ||
           (len == 19 && strncmp(name, "java/io/PrintStream", 19) == 0);
}

u4 dispatch_printstream(JVMThread* thread, ReferenceMap* reference_map, u2 method_name_len, char *descriptor, u4 pc) {
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    u1 is_println = (method_name_len == 7); // "print"=5, "println"=7

    char param = descriptor[1]; // ')' se vazio

    switch (param) {
        case ')': break; // sem argumento

        case 'I': {
            int32_t v = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            printf("%d", v);
            break;
        }
        case 'C': {
            u4 v = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            printf("%c", (char)(v & 0xFFFF));
            break;
        }
        case 'F': {
            u4 raw = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            union { u4 u; float f; } val = { raw };
            print_java_float(val.f);
            break;
        }
        case 'D': {
            u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            union { uint64_t u; double d; } val = { ((uint64_t)high << 32) | low };
            print_java_double(val.d);
            break;
        }
        case 'J': {
            u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            int64_t v = (int64_t)(((uint64_t)high << 32) | low);
            printf("%lld", (long long)v);
            break;
        }
        case 'L': { // Ljava/lang/String;
            u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            char *str = (char*) reference_map->entries[ref_key];
            printf("%s", str ? str : "null");
            break;
        }
        default:
            fprintf(stderr, "println/print: descriptor não suportado '%s'\n", descriptor);
            exit(1);
    }

    if (is_println) printf("\n");

    pop(frame->operand_stack); // descarta o receiver (vindo do getstatic)
    return pc + 3;
}

u4 dispatch_stringbuffer(JVMThread *thread, ReferenceMap *reference_map, u2 method_name_len, char *method_name, char *descriptor, u4 pc) {
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    // append(Ljava/lang/String;)Ljava/lang/StringBuffer;
    if (method_name_len == 6 && strncmp(method_name, "append", 6) == 0) {
        u4 str_ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
        u4 buf_ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

        char *str = (char*) reference_map->entries[str_ref];
        JVMStringBuffer *buf = (JVMStringBuffer*) reference_map->entries[buf_ref];

        size_t str_len = str ? strlen(str) : 4; // "null" se a string ref for null
        size_t needed = buf->length + str_len + 1;

        if (needed > buf->capacity) {
            while (buf->capacity < needed) buf->capacity *= 2;
            buf->data = (char*) realloc(buf->data, buf->capacity);
        }

        strcpy(buf->data + buf->length, str ? str : "null");
        buf->length += str_len;

        push(frame->operand_stack, (void*)&buf_ref); // append retorna 'this'
        return pc + 3;
    }

    // toString()Ljava/lang/String;
    if (method_name_len == 8 && strncmp(method_name, "toString", 8) == 0) {
        u4 buf_ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
        JVMStringBuffer *buf = (JVMStringBuffer*) reference_map->entries[buf_ref];

        char *result = (char*) malloc(buf->length + 1);
        memcpy(result, buf->data, buf->length + 1);

        if (reference_map->size >= MAX_REF_MAP) { printf("OutOfMemoryError: ReferenceMap cheio!\n"); exit(1); }
        u4 ref_key = reference_map->size++;
        reference_map->entries[ref_key] = (void*) result;
        push(frame->operand_stack, (void*)&ref_key);
        return pc + 3;
    }

    fprintf(stderr, "StringBuffer: método não suportado '%.*s'\n", method_name_len, method_name);
    exit(1);
}

static Code_attribute *ParseCodeAttribute(Attribute_info *attr);

Code_attribute* getCodeAttributeFromTopFrame(Stack* frame_stack) {
    if (frame_stack == NULL)
        return NULL;

    Frame *frame = (Frame*) getTop(frame_stack);
    if (frame == NULL || frame->method == NULL || frame->class_file == NULL)
        return NULL;

    Method_info *method = frame->method;

    for (u2 i = 0; i < method->attributes_count; i++) {

        Attribute_info *attr = &method->attributes[i];

        u2 name_index = attr->attribute_name_index;

        Cp_info *cp = &frame->class_file->constant_pool[name_index];

        if (cp->tag != CONSTANT_Utf8)
            continue;

        if (cp->info.Utf8.length == 4 &&
            memcmp(cp->info.Utf8.bytes, "Code", 4) == 0)
        {
            return ParseCodeAttribute(attr);
        }
    }

    return NULL;
}

static u1 read_u1(u1 **ptr) {
    return *(*ptr)++;
}

static u2 read_u2(u1 **ptr) {
    u2 value = ((u2)(*ptr)[0] << 8) |
               ((u2)(*ptr)[1]);

    *ptr += 2;
    return value;
}

static u4 read_u4(u1 **ptr) {
    u4 value = ((u4)(*ptr)[0] << 24) |
               ((u4)(*ptr)[1] << 16) |
               ((u4)(*ptr)[2] << 8)  |
               ((u4)(*ptr)[3]);

    *ptr += 4;
    return value;
}

static Code_attribute *ParseCodeAttribute(Attribute_info *attr) {
    if (attr == NULL || attr->info == NULL)
        return NULL;

    u1 *ptr = attr->info;

    Code_attribute *code = calloc(1, sizeof(Code_attribute));
    if (code == NULL)
        return NULL;

    code->attribute_name_index = attr->attribute_name_index;
    code->attribute_length     = attr->attribute_length;

    code->max_stack  = read_u2(&ptr);
    code->max_locals = read_u2(&ptr);

    code->code_length = read_u4(&ptr);

    if (code->code_length > 0) {
        code->code = malloc(code->code_length);

        if (code->code == NULL) {
            free(code);
            return NULL;
        }

        memcpy(code->code, ptr, code->code_length);
        ptr += code->code_length;
    }

    code->exception_table_length = read_u2(&ptr);

    if (code->exception_table_length > 0) {

        code->exception_table =
            malloc(sizeof(Exception_code) * code->exception_table_length);

        if (code->exception_table == NULL) {
            free(code->code);
            free(code);
            return NULL;
        }

        for (u2 i = 0; i < code->exception_table_length; i++) {
            code->exception_table[i].start_pc   = read_u2(&ptr);
            code->exception_table[i].end_pc     = read_u2(&ptr);
            code->exception_table[i].handler_pc = read_u2(&ptr);
            code->exception_table[i].catch_type = read_u2(&ptr);
        }
    }

    code->attributes_count = read_u2(&ptr);

    if (code->attributes_count > 0) {

        code->attributes =
            malloc(sizeof(Attribute_info) * code->attributes_count);

        if (code->attributes == NULL) {
            free(code->exception_table);
            free(code->code);
            free(code);
            return NULL;
        }

        for (u2 i = 0; i < code->attributes_count; i++) {

            code->attributes[i].attribute_name_index = read_u2(&ptr);
            code->attributes[i].attribute_length     = read_u4(&ptr);

            u4 len = code->attributes[i].attribute_length;

            if (len > 0) {

                code->attributes[i].info = malloc(len);

                if (code->attributes[i].info == NULL) {

                    for (u2 j = 0; j < i; j++)
                        free(code->attributes[j].info);

                    free(code->attributes);
                    free(code->exception_table);
                    free(code->code);
                    free(code);

                    return NULL;
                }

                memcpy(code->attributes[i].info, ptr, len);
                ptr += len;
            } else {
                code->attributes[i].info = NULL;
            }
        }
    }

    return code;
}

void FreeCodeAttribute(Code_attribute *code) {
    if (code == NULL)
        return;

    free(code->code);

    if (code->attributes != NULL) {
        for (u2 i = 0; i < code->attributes_count; i++) {
            free(code->attributes[i].info);
        }
        free(code->attributes);
    }

    free(code->exception_table);

    free(code);
}

u1 IsMethodNamed(Method_info *method, ClassFile *class_file, const char *name, size_t len) {
    u2 idx = method->name_index;

    if (idx == 0 || idx >= class_file->constant_pool_count)
        return 0;

    Cp_info *cp = &class_file->constant_pool[idx];

    return (cp->tag == CONSTANT_Utf8 && cp->info.Utf8.length == len && memcmp(cp->info.Utf8.bytes, name, len) == 0);
}

void MarkClinitDoneIfApplicable(MethodArea* method_area, Frame *frame) {
    if (IsMethodNamed(frame->method, frame->class_file, "<clinit>", 8)) {
        char *class_name = GetClassName(frame->class_file, frame->class_file->this_class);
        MethodAreaEntry *entry = MethodAreaGetEntry(method_area, class_name);
        if (entry != NULL) entry->state = CLASS_INITIALIZED;
    }
}

// Busca exata (nome+descritor) só nos métodos declarados em cf, sem subir hierarquia.
static Method_info* FindMethodExact(ClassFile *cf, const char *name, size_t name_len,
                                     const char *descriptor, size_t descriptor_len) {
    if (cf == NULL) return NULL;

    for (u2 i = 0; i < cf->methods_count; i++) {
        Method_info *method = &cf->methods[i];
        Cp_info *name_cp = &cf->constant_pool[method->name_index];
        Cp_info *desc_cp = &cf->constant_pool[method->descriptor_index];
        if (name_cp->info.Utf8.length == name_len &&
            memcmp(name_cp->info.Utf8.bytes, name, name_len) == 0 &&
            desc_cp->info.Utf8.length == descriptor_len &&
            memcmp(desc_cp->info.Utf8.bytes, descriptor, descriptor_len) == 0) {
            return method;
        }
    }
    return NULL;
}

// Procura nome+descritor em start_class e, se não achar, sobe a cadeia de superclasses.
// out_owner recebe a ClassFile que de fato declara o método (Method_info não guarda
// isso, então precisamos rastrear separadamente — necessário pra pushFrame depois).
Method_info* ResolveMethod(MethodArea *method_area, ClassFile *start_class,
                            const char *name, size_t name_len,
                            const char *descriptor, size_t descriptor_len,
                            ClassFile **out_owner) {
    ClassFile *cf = start_class;

    while (cf != NULL) {
        Method_info *method = FindMethodExact(cf, name, name_len, descriptor, descriptor_len);
        if (method != NULL) {
            if (out_owner != NULL) *out_owner = cf;
            return method;
        }

        if (cf->super_class == 0) break;
        char *super_name = GetClassName(cf, cf->super_class);
        if (super_name == NULL) break;

        MethodAreaEntry *super_entry = MethodAreaGetEntry(method_area, super_name);
        cf = (super_entry != NULL) ? super_entry->class_file : NULL;
    }
    return NULL;
}

// Largura (em slots de 32 bits) de cada parâmetro do descritor, na ordem em que
// aparecem. J/D ocupam 2 slots; o resto (refs e arrays são sempre 1 ref_key) ocupa 1.
u2 ParseArgWidths(const char *descriptor, u1 *widths, u2 *out_total_slots) {
    u2 n_args = 0, total = 0;
    const char *p = descriptor;

    if (p == NULL || *p != '(') { if (out_total_slots) *out_total_slots = 0; return 0; }
    p++;

    while (*p != ')') {
        u1 width = 1;
        switch (*p) {
            case 'J': case 'D': width = 2; p++; break;
            case 'L': p++; while (*p != ';') p++; p++; break;
            case '[':
                while (*p == '[') p++;
                if (*p == 'L') { while (*p != ';') p++; }
                p++;
                break;
            default: p++; break; // B C F I S Z
        }
        widths[n_args++] = width;
        total += width;
    }

    if (out_total_slots) *out_total_slots = total;
    return n_args;
}

// Desempilha os argumentos (NÃO o objectref) da operand stack do chamador, conforme
// o descritor, e escreve em arg_words na ordem em que vão para as locals do callee
// (a partir do índice 1, depois do objectref). Retorna o total de slots usados.
u2 PopArguments(Frame *caller_frame, const char *descriptor, u4 *arg_words) {
    u1 widths[256];
    u2 total_slots;
    u2 n_args = ParseArgWidths(descriptor, widths, &total_slots);
    u2 offset = total_slots;

    for (int i = n_args - 1; i >= 0; i--) {
        u1 w = widths[i];
        offset -= w;

        if (w == 2) {
            u4 low  = *((u4*) getTop(caller_frame->operand_stack)); pop(caller_frame->operand_stack);
            u4 high = *((u4*) getTop(caller_frame->operand_stack)); pop(caller_frame->operand_stack);
            arg_words[offset]     = high;
            arg_words[offset + 1] = low;
        } else {
            u4 v = *((u4*) getTop(caller_frame->operand_stack)); pop(caller_frame->operand_stack);
            arg_words[offset] = v;
        }
    }
    return total_slots;
}

// Empilha um Frame novo para 'method' (declarado em 'owner'), reaproveitando pushFrame
// (que já lê max_stack/max_locals do atributo Code). local[0] = objectref,
// local[1..n] = argumentos. Retorna 0 — mesmo motivo de InitializeClass fazer
// thread->pc = 0 ao empilhar um <clinit>: pc é global da thread, não por frame.
u4 PushUserMethodFrame(JVMThread *thread, ClassFile *owner, Method_info *method,
                               u4 objectref, u4 *arg_words, u2 total_slots, u4 return_pc) {
    pushFrame(thread, owner, method, return_pc);

    Frame *new_frame = (Frame*) getTop(thread->frame_stack);
    new_frame->local_variables[0] = objectref;
    for (u2 i = 0; i < total_slots; i++) {
        new_frame->local_variables[1 + i] = arg_words[i];
    }
    return 0;
}