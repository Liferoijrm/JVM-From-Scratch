/**
 * @file printer.h
 * @brief Definições e declarações para a exportação e exibição da estrutura de um arquivo .class.
 */

#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
#include <string.h>
#include "./classviewer.h"
#include "../class_loader/loading/classparser.h"

/**
 * @def CLEAR
 * @brief Macro para o comando do sistema de limpar o terminal, adaptável ao sistema operacional.
 */
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear" 
#endif

/**
 * @brief Imprime as informações detalhadas de um ClassFile em um arquivo de texto.
 * * A função extrai o nome da classe do Constant Pool e cria um arquivo `.txt`
 * correspondente. Ela resolve conflitos de diretório substituindo barras (`/`)
 * por underlines (`_`) em nomes de pacotes (ex: java/lang/Object -> java_lang_Object.txt).
 * * Após definir o arquivo, a saída padrão (stdout) é redirecionada para ele, e 
 * as seções da classe (Constant Pool, Interfaces, Fields, Methods, Attributes) 
 * são impressas sequencialmente.
 * * @warning Esta função altera o fluxo de saída padrão (stdout) utilizando `freopen` 
 * e o fecha com `fclose(stdout)` no final da execução. Qualquer tentativa de usar 
 * `printf` no terminal após a chamada desta função falhará, a menos que o 
 * stdout seja reaberto.
 * * @param cf Ponteiro para a estrutura ClassFile a ser processada e impressa.
 */
void printClass(ClassFile *cf);

#endif