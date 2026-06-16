#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "../../runtime_data/method_area/methodarea.h"
#include "../loading/classparser.h"

u1 InitializeClass(MethodArea *method_area, ClassFile *class_file);

#endif