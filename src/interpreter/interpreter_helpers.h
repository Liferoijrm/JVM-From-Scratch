/**
 * @file interpreter_helpers.h
 * @brief Declarações das funções auxiliares do interpretador de bytecode.
 */

#ifndef INTERPRETER_HELPERS_H
#define INTERPRETER_HELPERS_H

#include "../utils/utils.h"
#include "../runtime_data/allocation/allocation.h"
#include "../runtime_data/method_area/methodarea.h"
#include "../runtime_data/thread_data/thread_data.h"
#include "../class_loader/loading/classparser.h"
#include "../class_loader/loading/classloader.h"
#include "../class_loader/initialization/initialization.h"
#include "../class_loader/linking/linking.h"

/**
 * @brief Obtém o atributo Code do frame no topo da pilha.
 * @param frame_stack Pilha de frames da thread.
 * @return Ponteiro para o atributo Code, ou NULL se não encontrado.
 */
Code_attribute* getCodeAttributeFromTopFrame(Stack* frame_stack);

/**
 * @brief Libera a memória de um atributo Code.
 * @param code Ponteiro para o atributo Code a ser liberado.
 */
void FreeCodeAttribute(Code_attribute *code);

/**
 * @brief Obtém o nome de uma classe a partir do constant pool.
 * @param class_file Ponteiro para o ClassFile.
 * @param this_class Índice no constant pool.
 * @return Ponteiro para a string do nome da classe.
 */
char* GetClassName(ClassFile* class_file, u2 this_class);

/**
 * @brief Marca o <clinit> como concluído se o frame atual for o inicializador.
 * @param method_area Ponteiro para a Method Area.
 * @param frame Ponteiro para o frame atual.
 */
void MarkClinitDoneIfApplicable(MethodArea* method_area, Frame *frame);

/**
 * @brief Resolve uma referência a método no constant pool.
 * @param method_area Ponteiro para a Method Area.
 * @param start_class Classe inicial para busca.
 * @param name Nome do método.
 * @param name_len Comprimento do nome.
 * @param descriptor Descritor do método.
 * @param descriptor_len Comprimento do descritor.
 * @param out_owner Ponteiro de saída para a classe proprietária do método.
 * @return Ponteiro para o Method_info encontrado, ou NULL.
 */
Method_info* ResolveMethod(MethodArea *method_area, ClassFile *start_class, const char *name, size_t name_len, const char *descriptor, size_t descriptor_len, ClassFile **out_owner);

/**
 * @brief Analisa um descritor de método para extrair larguras dos argumentos.
 * @param descriptor String do descritor.
 * @param widths Array de saída com as larguras de cada argumento.
 * @param out_total_slots Ponteiro de saída para o total de slots.
 * @return Número de argumentos.
 */
u2 ParseArgWidths(const char *descriptor, u1 *widths, u2 *out_total_slots);

/**
 * @brief Desempilha os argumentos de um frame chamador.
 * @param caller_frame Frame do método chamador.
 * @param descriptor Descritor do método.
 * @param arg_words Array de saída com as palavras dos argumentos.
 * @return Número de palavras desempilhadas.
 */
u2 PopArguments(Frame *caller_frame, const char *descriptor, u4 *arg_words);

/**
 * @brief Cria e empilha um novo frame para um método chamado.
 * @param thread Ponteiro para a thread.
 * @param owner ClassFile do método proprietário.
 * @param method Ponteiro para o método.
 * @param objectref Referência ao objeto (para métodos de instância).
 * @param arg_words Array de palavras dos argumentos.
 * @param total_slots Total de slots dos argumentos.
 * @param return_pc PC de retorno.
 * @return O valor do PC de retorno.
 */
u4 PushUserMethodFrame(JVMThread *thread, ClassFile *owner, Method_info *method, u4 objectref, u4 *arg_words, u2 total_slots, u4 return_pc);

/**
 * @brief Verifica se um método possui um nome específico.
 * @param method Ponteiro para o método.
 * @param class_file Ponteiro para o ClassFile.
 * @param name Nome a ser comparado.
 * @param len Comprimento do nome.
 * @return 1 se o nome corresponde, 0 caso contrário.
 */
u1 IsMethodNamed(Method_info *method, ClassFile *class_file, const char *name, size_t len);

/**
 * @brief Obtém a classe a partir de uma referência no constant pool.
 * @param thread Ponteiro para a thread.
 * @param method_area Ponteiro para a Method Area.
 * @param frame Ponteiro para o frame atual.
 * @param class_name Nome da classe.
 * @return Ponteiro para o ClassFile, ou NULL.
 */
ClassFile* get_class_from_constant_pool(JVMThread* thread, MethodArea* method_area, Frame* frame, char* class_name);

/**
 * @brief Despacha chamadas nativas para PrintStream.
 * @param thread Ponteiro para a thread.
 * @param reference_map Ponteiro para o mapa de referências.
 * @param method_name_len Comprimento do nome do método.
 * @param descriptor Descritor do método.
 * @param pc PC atual.
 * @return Próximo PC.
 */
u4 dispatch_printstream(JVMThread* thread, ReferenceMap* reference_map, u2 method_name_len, char *descriptor, u4 pc);

/**
 * @brief Despacha chamadas nativas para StringBuffer.
 * @param thread Ponteiro para a thread.
 * @param reference_map Ponteiro para o mapa de referências.
 * @param method_name_len Comprimento do nome do método.
 * @param method_name Nome do método.
 * @param descriptor Descritor do método.
 * @param pc PC atual.
 * @return Próximo PC.
 */
u4 dispatch_stringbuffer(JVMThread *thread, ReferenceMap *reference_map, u2 method_name_len, char *method_name, char *descriptor, u4 pc);

/**
 * @brief Verifica se um nome de classe corresponde a uma classe nativa.
 * @param name Nome da classe.
 * @param len Comprimento do nome.
 * @return 1 se for classe nativa, 0 caso contrário.
 */
u1 is_native_class(const char *name, u2 len);

#endif