#include "methodarea.h"

#include <stdio.h>

#define METHOD_AREA_INITIAL_CAPACITY 4

static char *DuplicateString(const char* source) {
	size_t length = strlen(source) + 1;
	char *copy = (char *)malloc(length);

	if(copy) {
		memcpy(copy, source, length);
	}

	return copy;
}

static const char* GetClassNameFromClassFile(const ClassFile* class_file) {
	u2 class_name_index;

	if(!class_file || !class_file->constant_pool || class_file->this_class >= class_file->constant_pool_count) {
		return NULL;
	}

	if(class_file->constant_pool[class_file->this_class].tag != CONSTANT_Class) {
		return NULL;
	}

	class_name_index = class_file->constant_pool[class_file->this_class].info.Class.name_index;

	if(class_name_index == 0 || class_name_index >= class_file->constant_pool_count) {
		return NULL;
	}

	if(class_file->constant_pool[class_name_index].tag != CONSTANT_Utf8) {
		return NULL;
	}

	return (const char*)class_file->constant_pool[class_name_index].info.Utf8.bytes;
}

static u2 CountDeclaredInstanceFields(ClassFile *class_file) {
	u2 count = 0;

	if(!class_file || !class_file->fields) {
		return 0;
	}

	for(u2 i = 0; i < class_file->fields_count; i++) {
		if((class_file->fields[i].access_flags & 0x0008) == 0) {
			count++;
		}
	}

	return count;
}

static u2 CountDeclaredStaticFields(ClassFile *class_file) {
	u2 count = 0;

	if(!class_file || !class_file->fields) {
		return 0;
	}

	for(u2 i = 0; i < class_file->fields_count; i++) {
		if(class_file->fields[i].access_flags & 0x0008) {
			count++;
		}
	}

	return count;
}

static u1 EnsureCapacity(MethodArea* method_area, u2 minimum_capacity) {
	MethodAreaEntry* new_entries;
	u2 new_capacity;

	if(!method_area) {
		return METHOD_AREA_NULL_POINTER;
	}

	if(method_area->capacity >= minimum_capacity) {
		return METHOD_AREA_OK;
	}

	new_capacity = method_area->capacity ? method_area->capacity : METHOD_AREA_INITIAL_CAPACITY;
	while(new_capacity < minimum_capacity) {
		new_capacity *= 2;
	}

	new_entries = (MethodAreaEntry*)realloc(method_area->entries, new_capacity * sizeof(MethodAreaEntry));
	if(!new_entries) {
		return METHOD_AREA_ALLOC_ERROR;
	}

	for(u2 i = method_area->capacity; i < new_capacity; i++) {
		new_entries[i].class_name = NULL;
		new_entries[i].class_file = NULL;
		new_entries[i].static_fields = NULL;
		new_entries[i].static_field_count = 0;
	}

	method_area->entries = new_entries;
	method_area->capacity = new_capacity;

	return METHOD_AREA_OK;
}

MethodArea* CreateMethodArea(void) {
	MethodArea* method_area = (MethodArea *)calloc(1, sizeof(MethodArea));

	if(!method_area) {
		return NULL;
	}

	return method_area;
}

void DestroyMethodArea(MethodArea* method_area) {
	if(!method_area) {
		return;
	}
	if(method_area->entries) {
		for (u2 i = 0; i < method_area->count; i++) {
			if (method_area->entries[i].class_name) {
				free(method_area->entries[i].class_name);
			}

			free(method_area->entries[i].static_fields);

			if (method_area->entries[i].class_file) {
				FreeClass(method_area->entries[i].class_file);
			}
		}
		if(method_area->entries != NULL) free(method_area->entries);
	}

	free(method_area);
}

u1 MethodAreaAddClass(MethodArea* method_area, ClassFile* class_file) {
	const char* class_name;
	char* class_name_copy;
	u1 status;

	if(!method_area || !class_file) {
		return METHOD_AREA_NULL_POINTER;
	}

	class_name = GetClassNameFromClassFile(class_file);
	if(!class_name) {
		return METHOD_AREA_INVALID_CLASS;
	}

	if(MethodAreaFindClass(method_area, class_name)) {
		return METHOD_AREA_DUPLICATE_CLASS;
	}

	status = EnsureCapacity(method_area, (u2)(method_area->count + 1));
	if(status != METHOD_AREA_OK) {
		return status;
	}

	class_name_copy = DuplicateString(class_name);
	if(!class_name_copy) {
		return METHOD_AREA_ALLOC_ERROR;
	}

	method_area->entries[method_area->count].class_name = class_name_copy;
	method_area->entries[method_area->count].class_file = class_file;
	method_area->entries[method_area->count].static_fields = NULL;
	method_area->entries[method_area->count].static_field_count = 0;
	method_area->count++;

	status = MethodAreaPrepareClass(method_area, class_file);
	return status;
}

u1 MethodAreaPrepareClass(MethodArea *method_area, ClassFile *class_file) {
	const char *class_name;
	u2 static_count;

	if(!method_area || !class_file) {
		return METHOD_AREA_NULL_POINTER;
	}

	class_name = GetClassNameFromClassFile(class_file);
	if(!class_name) {
		return METHOD_AREA_INVALID_CLASS;
	}

	static_count = MethodAreaCountStaticFields(method_area, class_file);

	for(u2 i = 0; i < method_area->count; i++) {
		if(method_area->entries[i].class_file == class_file) {
			method_area->entries[i].static_field_count = static_count;
			if(static_count > 0) {
				method_area->entries[i].static_fields = (u4 *)calloc(static_count, sizeof(u4));
				if(!method_area->entries[i].static_fields) {
					return METHOD_AREA_ALLOC_ERROR;
				}
			}
			return METHOD_AREA_OK;
		}
	}

	return METHOD_AREA_INVALID_CLASS;
}

ClassFile *MethodAreaFindClass(const MethodArea* method_area, const char* class_name) {
	if(!method_area || !class_name) {
		return NULL;
	}

	for(u2 i = 0; i < method_area->count; i++) {
		if (method_area->entries[i].class_name && strcmp(method_area->entries[i].class_name, class_name) == 0) {
			return method_area->entries[i].class_file;
		}
	}

	return NULL;
}

u2 MethodAreaCount(const MethodArea* method_area) {
	if(!method_area) {
		return 0;
	}

	return method_area->count;
}

u2 MethodAreaCountInstanceFields(const MethodArea *method_area, ClassFile *class_file) {
	u2 count = CountDeclaredInstanceFields(class_file);
	char *super_name;
	ClassFile *super_class_file;

	if(!method_area || !class_file || class_file->super_class == 0) {
		return count;
	}

	super_name = GetClassName(class_file, class_file->super_class);
	if(!super_name) {
		return count;
	}

	super_class_file = MethodAreaFindClass(method_area, super_name);
	if(!super_class_file || super_class_file == class_file) {
		return count;
	}

	return (u2)(count + MethodAreaCountInstanceFields(method_area, super_class_file));
}

u2 MethodAreaCountStaticFields(const MethodArea *method_area, ClassFile *class_file) {
	u2 count = CountDeclaredStaticFields(class_file);
	char *super_name;
	ClassFile *super_class_file;

	if(!method_area || !class_file || class_file->super_class == 0) {
		return count;
	}

	super_name = GetClassName(class_file, class_file->super_class);
	if(!super_name) {
		return count;
	}

	super_class_file = MethodAreaFindClass(method_area, super_name);
	if(!super_class_file || super_class_file == class_file) {
		return count;
	}

	return (u2)(count + MethodAreaCountStaticFields(method_area, super_class_file));
}

MethodAreaEntry* MethodAreaGetEntry(MethodArea* ma, const char* class_name){
	if(ma == NULL || class_name == NULL){ 
		return NULL;
	}

	// Busca a classe pelo nome linearmente na MethodArea
	for(u2 i = 0 ; i < ma-> count ; i++){
		if(strcmp(ma->entries[i].class_name, class_name) == 0) return &ma->entries[i];
	}

	return NULL; // Classe não encontrada
}