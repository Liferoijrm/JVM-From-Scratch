#include <stdio.h>
#include "../class_loader/loading/classparser.h"

void PrintCpool(Cp_info *cpool, u2 count);

void PrintFields(Cp_info *cpool, Field_info *fields, u2 count);

void PrintMethods(Cp_info *cpool, Method_info *methods, u2 count);

void PrintAttributes(Cp_info *cpool, Attribute_info *attributes, u2 count);

void DecodeDescriptor(u1 *descriptor, u2 len);