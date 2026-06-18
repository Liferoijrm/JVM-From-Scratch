#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "../../runtime_data/method_area/methodarea.h"

u1 ResolveClass(MethodAreaEntry *entry);

u1 ResolveSymbolicRef(MethodAreaEntry *entry, u2 cp_index, const MethodArea *ma);

#endif