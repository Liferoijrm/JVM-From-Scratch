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

static void MarkClinitDoneIfApplicable(MethodArea* method_area, Frame *frame);

Method_info* ResolveMethod(MethodArea *method_area, ClassFile *start_class, const char *name, size_t name_len, const char *descriptor, size_t descriptor_len, ClassFile **out_owner);

u2 ParseArgWidths(const char *descriptor, u1 *widths, u2 *out_total_slots);

u2 PopArguments(Frame *caller_frame, const char *descriptor, u4 *arg_words);

u4 PushUserMethodFrame(JVMThread *thread, ClassFile *owner, Method_info *method, u4 objectref, u4 *arg_words, u2 total_slots, u4 return_pc);

u1 IsMethodNamed(Method_info *method, ClassFile *class_file, const char *name, size_t len);

ClassFile* get_class_from_constant_pool(JVMThread* thread, MethodArea* method_area, Frame* frame, char* class_name);

u4 dispatch_printstream(JVMThread* thread, ReferenceMap* reference_map, u2 method_name_len, char *descriptor, u4 pc);

u4 dispatch_stringbuffer(JVMThread *thread, ReferenceMap *reference_map, u2 method_name_len, char *method_name, char *descriptor, u4 pc);

u1 is_native_class(const char *name, u2 len);

#endif