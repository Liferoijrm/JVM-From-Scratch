// src/class_loader/linking/linking.c
#include "linking.h"
#include "verifier.h"
#include "preparation.h"
#include "resolution.h"
#include <stdio.h>

u1 LinkClass(MethodAreaEntry *entry, const MethodArea *ma) {
    if (entry == NULL || entry->class_file == NULL) return 0;

    if (entry->state == CLASS_LINKED || entry->state == CLASS_INITIALIZING || entry->state == CLASS_INITIALIZED){
        return 1;
    }

    if (!VerifyClass(entry->class_file, ma)){
        return 0;
    }

    PrepareClass(entry);

    if (!ResolveClass(entry)){
        return 0;
    }

    entry->state = CLASS_LINKED;

    return 1;
}