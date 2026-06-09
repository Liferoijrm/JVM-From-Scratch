#include "loadedclass.h"
#include <stdlib.h>

LoadedClass *CreateLoadedClass(ClassFile *cf) {

    if (cf == NULL)
        return NULL;

    LoadedClass *loaded =
        (LoadedClass *) calloc(1, sizeof(LoadedClass));

    if (loaded == NULL)
        return NULL;

    loaded->class_file = cf;

    loaded->linked = 0;
    loaded->initialized = 0;

    return loaded;
}

void DestroyLoadedClass(LoadedClass *loaded) {

    if (loaded == NULL)
        return;

    FreeClass(loaded->class_file);

    free(loaded);
}