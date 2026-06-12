#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>
#include <stdint.h>

#include "../method_area/methodarea.h"

#define JVM_ATYPE_BOOLEAN 4
#define JVM_ATYPE_CHAR 5
#define JVM_ATYPE_FLOAT 6
#define JVM_ATYPE_DOUBLE 7
#define JVM_ATYPE_BYTE 8
#define JVM_ATYPE_SHORT 9
#define JVM_ATYPE_INT 10
#define JVM_ATYPE_LONG 11

typedef struct JVMObject {
	ClassFile *class_ref;
	u4 *fields;
} JVMObject;

typedef struct JVMArray {
	u4 length;
	u1 atype;
	void *data;
} JVMArray;

JVMObject *CreateObject(const MethodArea *method_area, ClassFile *class_file);

JVMArray *CreatePrimitiveArray(u1 atype, int32_t length);

size_t ArrayTypeSize(u1 atype);

#endif