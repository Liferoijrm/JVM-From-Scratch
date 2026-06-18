#ifndef EXEC_LOOP_H
#define EXEC_LOOP_H

#include "../utils/utils.h"

// ============================================================
// Public API for the bytecode execution loop
// ============================================================

// Executes a bytecode array from start to finish.
// Parameters:
//   code       - bytecode array
//   code_length- number of bytes in code
void execute_bytecode(u1 *code, u4 code_length);

#endif