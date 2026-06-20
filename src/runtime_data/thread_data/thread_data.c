#include "thread_data.h"
#include <string.h>

// funcao de inicializar a thread (com PC e stack ja criados)
JVMThread* createThread(u4 stack_max_size){
    JVMThread* thread = (JVMThread*) malloc(sizeof(JVMThread));
    thread->pc = 0; // valor padrao
    thread->frame_stack = createStack(stack_max_size, sizeof(Frame), freeFrame);
    return thread;
}

// cria e empilha um frame na thread atual
void pushFrame(JVMThread* thread, ClassFile* cf, Method_info* method, u4 return_pc){
    if(!thread || !cf || !method) return;

    Frame frame;
    frame.method = method;
    frame.class_file = cf;
    frame.return_pc = return_pc;

    u2 max_stack = 0;
    u2 max_locals = 0;

    // busca no atributo code os tamanhos de max_stack e max_locals
    for(u2 i = 0; i < method->attributes_count; i++){
        u2 name_index = method->attributes[i].attribute_name_index;
        char* attr_name = (char*) cf->constant_pool[name_index].info.Utf8.bytes;
        if(!strcmp(attr_name, "Code")){
            u1* info = method->attributes[i].info;
            max_stack  = (info[0] << 8) | info[1];
            max_locals = (info[2] << 8) | info[3];
            break;
        }
    }

    //TODO: juntar 2 slots p/ 64bits futuramente no interpretador creio eu
    // aloca variaveis locais e pilha de operandos
    if(max_locals > 0){
        frame.local_variables = (u4*) calloc(max_locals, sizeof(u4));
    } else {
        frame.local_variables = NULL;
    }
    frame.operand_stack = createStack(max_stack, sizeof(u4), free);

    push(thread->frame_stack, (void*)&frame);
}

// desempilha um frame da thread atual e retorna o pc para ela
void popFrame(JVMThread* thread){
    if(!thread || isEmpty(thread->frame_stack)) return;

    // resgata o topo para devolver o pc da thread
    Frame* current_frame = (Frame*) getTop(thread->frame_stack);
    thread->pc = current_frame->return_pc;
    pop(thread->frame_stack);
}

// funcao de free do frame
void freeFrame(void* frame_ptr){
    Frame* frame = (Frame*) frame_ptr;
    if(frame != NULL){
        free(frame->local_variables);
        freeStack(frame->operand_stack);
        free(frame);
    }
}

Code_attribute* getCodeAttributeFromTopFrame(Stack* frame_stack){
    Frame *frame = (Frame*) getTop(frame_stack);
    Method_info* method = frame->method;

    // busca nos atributos
    for(u2 i = 0; i < method->attributes_count; i++){
        u2 name_index = method->attributes[i].attribute_name_index;
        char* attr_name = (char*) frame->class_file->constant_pool[name_index].info.Utf8.bytes;
        if(!strcmp(attr_name, "Code")){
            return (Code_attribute*) method->attributes[i].info;
        }
    }
}