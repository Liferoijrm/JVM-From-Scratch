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
    if (entry->state < CLASS_INITIALIZED) {
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
    else printf("%g", f);
}

static void print_java_double(double d) {
    if (d == (long long)d && fabs(d) < 1e15) printf("%.1f", d);
    else printf("%g", d);
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