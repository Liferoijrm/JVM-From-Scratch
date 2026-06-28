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
 * * Esta função orquestra a leitura da estrutura interna da classe e a escreve
 * de forma formatada. Ela cria um arquivo .txt com o nome da classe lida.
 * * @param cf Ponteiro para a estrutura ClassFile previamente carregada na memória.
 */
void printClass(ClassFile *cf);

#endif