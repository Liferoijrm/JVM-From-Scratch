#include <stdio.h>
#include "../class_loader/loading/classparser.h"

void PrintCpool(Cp_info *cpool, u2 count);

void PrintFields(Field_info *fields, u2 count);

void PrintMethods(Method_info *methods, u2 count);

void PrintAttributes(Attribute_info *attributes, u2 count);