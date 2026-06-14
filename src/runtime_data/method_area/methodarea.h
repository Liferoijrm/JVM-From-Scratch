#ifndef METHODAREA_H
#define METHODAREA_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../../class_loader/loading/classparser.h"

#define METHOD_AREA_OK 0
#define METHOD_AREA_NULL_POINTER 1
#define METHOD_AREA_ALLOC_ERROR 2 
#define METHOD_AREA_INVALID_CLASS 3
#define METHOD_AREA_DUPLICATE_CLASS 4

typedef struct StaticField {
	u2 field_index;
	u4 value[2];
} StaticField;

typedef struct MethodAreaEntry {
	char *class_name;
	ClassFile *class_file;
	StaticField *static_fields;
	u2 static_field_count;
} MethodAreaEntry;

typedef struct MethodArea {
	MethodAreaEntry *entries;
	u2 count;
	u2 capacity;
} MethodArea;

MethodArea *CreateMethodArea(void);

void DestroyMethodArea(MethodArea *method_area);

u1 MethodAreaAddClass(MethodArea *method_area, ClassFile *class_file);

u1 MethodAreaPrepareClass(MethodArea *method_area, ClassFile *class_file);

ClassFile *MethodAreaFindClass(const MethodArea *method_area, const char *class_name);

u2 MethodAreaCount(const MethodArea *method_area);

u2 MethodAreaCountInstanceFields(const MethodArea *method_area, ClassFile *class_file);

u2 MethodAreaCountStaticFields(const MethodArea *method_area, ClassFile *class_file);

MethodAreaEntry* MethodAreaGetEntry(MethodArea* ma, const char* class_name);

#endif