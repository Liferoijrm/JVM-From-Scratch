#ifndef METHODAREA_H
#define METHODAREA_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../../class_loader/loading/classparser.h"

typedef struct MethodAreaEntry {
	char *class_name;
	ClassFile *class_file;
} MethodAreaEntry;

typedef struct MethodArea {
	MethodAreaEntry *entries;
	u2 count;
	u2 capacity;
} MethodArea;

MethodArea *CreateMethodArea(void);

void DestroyMethodArea(MethodArea *method_area);

u1 MethodAreaAddClass(MethodArea *method_area, ClassFile *class_file);

ClassFile *MethodAreaFindClass(const MethodArea *method_area, const char *class_name);

u2 MethodAreaCount(const MethodArea *method_area);

#endif
