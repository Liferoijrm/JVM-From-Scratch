#ifndef CLASSLOADER_H
#define CLASSLOADER_H

#include "classparser.h"
#include "../../runtime_data/method_area/methodarea.h"

ClassFile* LoadClass(MethodArea* ma, char* class_name);

#endif