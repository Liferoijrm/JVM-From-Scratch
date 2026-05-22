#ifndef CLASSVIEWER_H
#define CLASSVIEWER_H

#include <stdio.h>
#include <stdlib.h>
#include "../class_loader/loading/classparser.h"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear" 
#endif

void ViewClass(ClassFile *cf);

const char* GetJavaVersionText(u2 major);

void PrintClassFile(ClassFile *cf);

void PrintCpool(Cp_info *cpool, u2 count);

void PrintFields(Cp_info *cpool, Field_info *fields, u2 count);

void PrintMethods(Cp_info *cpool, Method_info *methods, u2 count);

void PrintAttributes(Cp_info *cpool, Attribute_info *attributes, u2 count);

void PrintInterfaces(Cp_info *cpool, u2 *interfaces, u2 count);

void DecodeDescriptor(u1 *descriptor, u2 len);

char* DecodeAccessFlags(u2 bitmask);

char* DecodeMethodAccessFlags(u2 bitmask);

void printModfiedUtf8(u1 *bytes, u2 length);

void cleanBuffer();


#endif