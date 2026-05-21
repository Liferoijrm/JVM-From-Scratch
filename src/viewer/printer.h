#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
#include <string.h>
#include "./classviewer.h"
#include "../class_loader/loading/classparser.h"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear" 
#endif

void printClass(ClassFile *cf);

#endif