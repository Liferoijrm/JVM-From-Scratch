#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../utils/utils.h"
#include "../runtime_data/allocation/allocation.h"
#include "../runtime_data/method_area/methodarea.h"
#include "../runtime_data/thread_data/thread_data.h"

typedef struct {
    JVMThread *thread;
    MethodArea *method_area;
    ReferenceMap *reference_map
} RuntimeContext;

// ============================================================
// Public API for the bytecode execution loop
// ============================================================

void interpret(RuntimeContext *ctx);

#endif