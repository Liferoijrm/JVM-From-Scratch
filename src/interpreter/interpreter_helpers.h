#ifndef INTERPRETER_HELPERS_H
#define INTERPRETER_HELPERS_H

#include "../utils/utils.h"
#include "../runtime_data/allocation/allocation.h"
#include "../runtime_data/method_area/methodarea.h"
#include "../runtime_data/thread_data/thread_data.h"
#include "../class_loader/loading/classparser.h"

Code_attribute* getCodeAttributeFromTopFrame(Stack* frame_stack);

void FreeCodeAttribute(Code_attribute *code);

char* GetClassName(ClassFile* class_file, u2 this_class);

u1 IsMethodNamed(Method_info *method, ClassFile *class_file, const char *name, size_t len);

ClassFile* get_class_from_constant_pool(JVMThread* thread, MethodArea* method_area, Frame* frame, char* class_name);

u4 dispatch_printstream(JVMThread* thread, ReferenceMap* reference_map, u2 method_name_len, char *descriptor, u4 pc);

u4 dispatch_stringbuffer(JVMThread *thread, ReferenceMap *reference_map, u2 method_name_len, char *method_name, char *descriptor, u4 pc);

u1 is_native_class(const char *name, u2 len);

#endif