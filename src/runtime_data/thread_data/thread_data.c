#include "thread_data.h"

// funcao de inicializar a thread (com PC e stack ja criados)
JVMThread* createThread(u4 stack_max_size){
    JVMThread* thread = (JVMThread*) malloc(sizeof(JVMThread));
    thread->pc = 0; // valor padrao
    thread->frame_stack = createStack(stack_max_size, sizeof(Frame), freeFrame);
    return thread;
}

// esses TODO podem ficar no interpreter tb

// TODO: funcao de criar frames (quando definir a method area e as instrucoes do interpreter que vao criar frames)
//      - Ver maxlocals, maxstack
//      - Parametros e valores na stack de 64 bits em 2 slots devem ser gerenciados

// TODO: funcao de dar pop/push no frame
//      - salvar return pc ao mudar de frame
//      - fazer as alteracoes corretas

// funcao de free do frame
void freeFrame(void* frame_ptr){
    Frame* frame = (Frame*) frame_ptr;
    if(frame != NULL){
        free(frame->local_variables);
        free(frame->operand_stack);
        free(frame);
    }
}

// TODO: outros kkkkkkk