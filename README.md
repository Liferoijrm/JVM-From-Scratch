# JVM-From-Scratch

Uma implementação da **Java Virtual Machine (JVM)** feita do zero em C, integrada com um **Bytecode Viewer** inspirado no **jclasslib**. Exploramos o carregamento de classes, a interpretação de bytecode e a semântica de execução definidas pela especificação da JVM.

> Site com a documentação (wiki) do projeto: https://neatzzy.github.io/JVM-From-Scratch/

---

## Índice

- [Sobre o projeto](#sobre-o-projeto)
- [Funcionalidades implementadas](#funcionalidades-implementadas)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Requisitos](#requisitos)
- [Compilando o projeto](#compilando-o-projeto)
  - [Linux / macOS](#linux--macos)
  - [Windows (MinGW + make)](#windows-mingw--make)
  - [Windows (compilação manual, sem make)](#windows-compilação-manual-sem-make)
  - [Compilação manual genérica](#compilação-manual-genérica-qualquer-sistema-operacional)
  - [Modo debug](#modo-debug)
- [Executando](#executando)
- [Gerando a documentação (Doxygen)](#gerando-a-documentação-doxygen)

---

## Sobre o projeto

O objetivo do projeto é implementar em C, sem o uso de bibliotecas prontas de JVM, todo o pipeline necessário para executar bytecode Java a partir de arquivos `.class`: leitura e parsing binário do formato `.class`, verificação e preparação (linking), inicialização de classes (`<clinit>`) e um interpretador de bytecode completo, rodando em modo **single-thread**.

### Desenvolvedores
 
| Desenvolvedor | GitHub |
|---|---|
| Pedro Marcinoni | [@Liferoijrm](https://github.com/Liferoijrm) |
| Rafael de Medeiros | [@leitaonerd](https://github.com/leitaonerd) |
| Elvis Miranda | [@neatzzy](https://github.com/neatzzy) |
| Davi Brasileiro | [@Redondave](https://github.com/Redondave) |
| Davi Galvão | [@daviggalvao](https://github.com/daviggalvao) |
| Athos Muniz | [@Athoscm2003](https://github.com/Athoscm2003) |

## Funcionalidades implementadas

### 🧩 Carregamento de classes (Class Loading)
- Parser completo do formato `.class`: magic number, versão, constant pool (todas as tags: `Class`, `Fieldref`, `Methodref`, `InterfaceMethodref`, `String`, `Integer`, `Float`, `Long`, `Double`, `NameAndType`, `Utf8`, `MethodHandle`, `MethodType`, `InvokeDynamic`), interfaces, fields, methods e atributos.
- Carregamento recursivo de superclasses e interfaces.
- Cache de classes já carregadas na **Method Area**, evitando releitura de disco.

### 🔗 Linkagem (Linking)
- **Verificação (Verification)**: validação de metadados (superclasse válida, interfaces herdando de `java/lang/Object`, proibição de herdar de classe `final`) e verificação de bytecode (tamanho de cada instrução, incluindo `tableswitch`/`lookupswitch` de tamanho variável, e checagem de limites da tabela de exceções).
- **Preparação (Preparation)**: alocação e zeragem dos campos estáticos de cada classe.

### 🚀 Inicialização (Initialization)
- Execução de `<clinit>` respeitando a ordem correta (a classe atual é empilhada antes da superclasse, garantindo execução LIFO consistente com a hierarquia).
- Estado `CLASS_INITIALIZING` para evitar dependências circulares durante a inicialização.
- Resolução dinâmica: quando o interpretador encontra uma classe ainda não inicializada (via `new`, `invokestatic`, `invokespecial`, `anewarray`, etc.), a instrução é pausada, o `<clinit>` correspondente é empilhado e executado, e a instrução original é reexecutada automaticamente assim que a classe estiver pronta.

### ⚙️ Interpretador de bytecode
- Tabela de dispatch (opcode → handler) cobrindo **quase a totalidade dos 202 opcodes** definidos pela especificação da JVM (exceto instruções multi-thread e invokedynamic): constantes, load/store, aritmética, operações bit a bit, conversões de tipo, comparações, controle de fluxo, chamadas de método, campos, arrays, e mais.
- Suporte completo a valores de 64 bits (`long`/`double`) através de pares de slots de 32 bits (*high/low*).
- `tableswitch` e `lookupswitch`, com padding e alinhamento tratados corretamente.
- `wide` para índices estendidos de variáveis locais.
- `multianewarray` com construção recursiva de arrays multidimensionais.

### 📞 Chamadas de método
- `invokestatic` e `invokespecial`: dispatch estático, resolvido a partir da classe referenciada no constant pool.
- `invokevirtual` e `invokeinterface`: dispatch **polimórfico**, resolvido a partir da classe real do objeto em tempo de execução.
- Resolução de métodos percorrendo toda a hierarquia de classes.

### 🧱 Campos
- `getstatic` / `putstatic`, armazenados na Method Area.
- `getfield` / `putfield`, com resolução correta de campos sombreados (*shadowing*), percorrendo toda a hierarquia de herança do objeto real para calcular o offset correto de cada slot.

### ☕ Suporte a classes nativas (mockadas)
- `java.lang.Object` (`registerNatives`)
- `java.lang.String` (criação via `ldc`/`ldc_w` e `new String()`)
- `java.io.PrintStream` — **`System.out.print` e `println`** para `int`, `char`, `float`, `double`, `long` e `String`
- `java.lang.StringBuffer` — **`append`** e **`toString`**

### 🧯 Tratamento de exceções
- `athrow` com busca de handler na exception table de cada frame, incluindo verificação de subclasse (`is_subclass_of`) percorrendo a hierarquia de classes do objeto lançado.

### 📦 Arrays
- `newarray` (tipos primitivos), `anewarray` (tipos de referência) e `multianewarray`.
- Todas as instruções de load/store de array: `iaload`/`iastore`, `laload`/`lastore`, `faload`/`fastore`, `daload`/`dastore`, `aaload`/`aastore`, `baload`/`bastore`, `caload`/`castore`, `saload`/`sastore`.
- `arraylength`.

### 🗺️ Reference Map
- Mapeamento de referências de 32 bits (categoria 1) para os ponteiros reais alocados na heap em C — usado para objetos, arrays, strings e `StringBuffer`s.

### 👁️ Leitor e visualizador de bytecode
- Módulo `viewer/` dedicado à leitura e exibição do conteúdo de um `.class` (constant pool, métodos, atributos etc.), independente do modo de execução.

---

## Estrutura do projeto

```
JVM-From-Scratch/
├── Examples/                       # Arquivos .class de exemplo (ex: Belote)
├── src/
│   ├── class_loader/
│   │   ├── loading/                # classparser.c/h, classloader.c/h
│   │   ├── linking/                # verifier.c/h, preparation.c/h
│   │   └── initialization/         # initialization.c/h (<clinit>)
│   ├── interpreter/
│   │   ├── interpreter.c/h         # loop principal de execução
│   │   ├── instruction_handler.c/h # tabela de dispatch e opcodes
│   │   └── interpreter_helpers.c/h # resolução de classes e natives
│   ├── runtime_data/
│   │   ├── method_area/            # Method Area (classes, campos estáticos)
│   │   ├── thread_data/            # JVMThread, Frame, pilha de frames
│   │   └── allocation/             # JVMObject, JVMArray, JVMStringBuffer
│   ├── utils/                      # tipos base (u1/u2/u4) e Stack genérica
│   └── viewer/                     # leitor/visualizador de bytecode
├── main.c
├── Makefile                        # build em Linux/macOS/MinGW (make)
├── Makefile.win                    # build em Windows (mingw32-make)
└── Doxyfile                        # configuração do Doxygen
```

---

## Requisitos

- Compilador C compatível com **C99** (GCC recomendado; MinGW no Windows)
- **GNU Make** (Linux/macOS) ou **mingw32-make** (Windows) — opcional, também é possível compilar manualmente
- `libm` (biblioteca matemática, linkada com `-lm`)
- *(opcional, para gerar documentação)* **Doxygen**
- *(opcional, para gerar documentação)* **Graphviz** (`dot`)

---

## Compilando o projeto

### Linux / macOS

```bash
git clone https://github.com/neatzzy/JVM-From-Scratch.git
cd JVM-From-Scratch
make
```

O executável será gerado em `bin/jvm`.

Outros alvos disponíveis:
```bash
make clean   # remove build/ e bin/
make debug   # recompila com -g -fsanitize=address,undefined
```

### Windows (MinGW + make)

Com o [MinGW](https://www.mingw-w64.org/) instalado e o `mingw32-make` disponível no PATH:

```
mingw32-make -f Makefile.win all
```

O executável será gerado em `bin\jvm.exe`.

Outros alvos disponíveis:
```
mingw32-make -f Makefile.win clean
mingw32-make -f Makefile.win debug
mingw32-make -f Makefile.win help
```

### Windows (compilação manual, sem make)

Via PowerShell, sem depender do Makefile:

```powershell
gcc -std=c99 -Isrc (Get-ChildItem -Path src -Recurse -Filter *.c | Select-Object -ExpandProperty FullName) -o jvm.exe -lm
```

### Compilação manual genérica (qualquer sistema operacional)

Para qualquer sistema com GCC e um shell POSIX (Linux, macOS, WSL, etc.):

```bash
gcc -std=c99 -Isrc $(find src -name '*.c') -o jvm -lm
```

### Modo debug

Ambos os Makefiles possuem um alvo `debug`, que recompila o projeto com símbolos de depuração e os sanitizers de endereço/comportamento indefinido habilitados (`-g -fsanitize=address,undefined`), útil para investigar vazamentos de memória e acessos inválidos.

---

## Executando

Basta passar o nome do arquivo `.class` (a ser executado ou apenas visualizado) para o executável gerado:

```bash
./bin/jvm run Belote      # executa o arquivo Belote.class
./bin/jvm view Belote     # exibe o conteúdo do Belote.class no Bytecode Viewer, armazenado em um .txt na pasta raiz do projeto
```

> O caminho para o arquivo `.class` é relativo ao diretório em que o comando é executado. Os `.class` a serem executados devem estar disponíveis no diretório `Examples/`.

---

## Gerando a documentação (Doxygen)

Este projeto usa o [Doxygen](https://doxygen.nl/) para gerar a documentação da API a partir dos comentários em estilo Javadoc presentes no código-fonte. O `Doxyfile` já vem configurado com `HAVE_DOT = YES`, ou seja, também gera diagramas de herança, colaboração e chamadas usando o **Graphviz**. Por isso, são necessárias **duas** ferramentas instaladas:

**1. Doxygen**
- Linux (Debian/Ubuntu): `sudo apt install doxygen`
- macOS (Homebrew): `brew install doxygen`
- Windows: instalador disponível em [doxygen.nl/download.html](https://www.doxygen.nl/download.html) (ou `choco install doxygen.install`)

**2. Graphviz (`dot`)**
- Linux (Debian/Ubuntu): `sudo apt install graphviz`
- macOS (Homebrew): `brew install graphviz`
- Windows: instalador disponível em [graphviz.org/download](https://graphviz.org/download/) (ou `choco install graphviz`)

> ⚠️ **Atenção (Windows)**: o `Doxyfile` define `DOT_PATH` apontando para `C:/Program Files (x86)/Graphviz/bin`. Se o Graphviz estiver instalado em outro local, ajuste esse caminho no `Doxyfile` — ou deixe o campo em branco caso o `dot` já esteja disponível no PATH do sistema.

Com as duas ferramentas instaladas, gere a documentação a partir da raiz do projeto:

```bash
doxygen Doxyfile
```

A saída será colocada no diretório `docs/`. Abra `docs/html/index.html` no navegador para visualizar a documentação gerada.
