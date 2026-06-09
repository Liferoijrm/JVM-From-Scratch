#ifndef LOADEDCLASS_H
#define LOADEDCLASS_H

#include "classparser.h"

typedef struct LoadedClass {
    ClassFile *class_file;

    unsigned char linked;
    unsigned char initialized;

} LoadedClass;

LoadedClass *CreateLoadedClass(ClassFile *cf);

void DestroyLoadedClass(LoadedClass *loaded);

#endif