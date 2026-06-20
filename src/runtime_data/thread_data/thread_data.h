#ifndef THREAD_DATA_H
#define THREAD_DATA_H

#include <stdlib.h>
#include "../../utils/utils.h"
#include "../../utils/stack.h"
#include "../../class_loader/loading/classparser.h"
// nao eh necessario fazer a struct da stack porque stack.h ja implementa

typedef struct JVMThread{
    u4 pc;
    Stack* frame_stack;
} JVMThread;

typedef struct Frame{
    u4* local_variables;
    Stack* operand_stack; // inicializar com tipo u4!
    u4 return_pc;

    Method_info* method; // TODO: ter estrutura MethodInfo nas estruturas da Method Area
    ClassFile* class_file; // acesso ao cpool temporario
} Frame;

JVMThread* createThread(u4 stack_max_size);
void pushFrame(JVMThread* thread, ClassFile* cf, Method_info* method, u4 return_pc);
void popFrame(JVMThread* thread);
void freeFrame(void* frame_ptr);
Code_attribute* getCodeAttributeFromTopFrame(Stack* frame_stack);

#endif