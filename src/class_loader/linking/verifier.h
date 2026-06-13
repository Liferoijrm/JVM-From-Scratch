#ifndef VERIFIER_H
#define VERIFIER_H

#include "../class_loader/loading/classparser.h"
#include "../runtime_data/method_area/methodarea.h"

// retorna 0 ou 1 para a verificação
u1 VerifyClass(ClassFile *cf, const MethodArea *ma);

#endif