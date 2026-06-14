#ifndef PREPARATION_H
#define PREPARTATION_H

#include <stddef.h>
#include <stdlib.h>
#include "../../runtime_data/method_area/methodarea.h"

#define ACC_STATIC 0x0008

void PrepareClass(MethodAreaEntry *entry);

#endif