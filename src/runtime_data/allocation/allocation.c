#include "allocation.h"

#include <stdlib.h>

size_t ArrayTypeSize(u1 atype) {
	switch(atype) {
		case JVM_ATYPE_BOOLEAN:
		case JVM_ATYPE_BYTE:
			return sizeof(uint8_t);
		case JVM_ATYPE_CHAR:
		case JVM_ATYPE_SHORT:
			return sizeof(uint16_t);
		case JVM_ATYPE_FLOAT:
		case JVM_ATYPE_INT:
			return sizeof(uint32_t);
		case JVM_ATYPE_DOUBLE:
		case JVM_ATYPE_LONG:
			return sizeof(uint64_t);
		default:
			return 0;
	}
}

JVMObject *CreateObject(const MethodArea *method_area, ClassFile *class_file) {
	JVMObject *object;
	u2 field_count;

	if(!class_file) {
		return NULL;
	}

	field_count = MethodAreaCountInstanceFields(method_area, class_file);
	object = (JVMObject *) calloc(1, sizeof(JVMObject));
	if(!object) {
		return NULL;
	}

	object->class_ref = class_file;

	if(field_count > 0) {
		object->fields = (u4 *) calloc(field_count, sizeof(u4));
		if(!object->fields) {
			free(object);
			return NULL;
		}
	}

	return object;
}

JVMArray *CreatePrimitiveArray(u1 atype, int32_t length) {
	JVMArray *array;
	size_t element_size;

	if(length < 0) {
		return NULL;
	}

	element_size = ArrayTypeSize(atype);
	if(element_size == 0) {
		return NULL;
	}

	array = (JVMArray *) calloc(1, sizeof(JVMArray));
	if(!array) {
		return NULL;
	}

	array->length = (u4) length;
	array->atype = atype;
	
	if(length > 0) {
		array->data = calloc((size_t) length, element_size);
		if(!array->data) {
			free(array);
			return NULL;
		}
	}

	return array;
}

