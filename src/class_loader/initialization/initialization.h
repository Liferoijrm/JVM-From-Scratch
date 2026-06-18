#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "../../runtime_data/method_area/methodarea.h"
#include "../../runtime_data/thread_data/thread_data.h"
#include "../loading/classparser.h"

u1 InitializeClass(MethodArea *method_area, MethodAreaEntry *entry, JVMThread *thread);

#endif