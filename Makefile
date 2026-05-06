CC            ?= gcc
CFLAGS        ?= -Wall -Wextra
DEBUG_FLAGS   ?= -g -fsanitize=address,undefined
SRC_DIR       := src
OBJ_DIR       := build
BIN_DIR       := bin
TARGET_NAME   := jvm

# Detectar o ambiente para definir a extensão do executável
UNAME_S := $(shell uname -s 2>/dev/null || echo Unknown)

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
EXE_EXT := .exe
else ifeq ($(findstring MSYS,$(UNAME_S)),MSYS)
EXE_EXT := .exe
else ifeq ($(findstring CYGWIN,$(UNAME_S)),CYGWIN)
EXE_EXT := .exe
else
EXE_EXT :=
endif

TARGET := $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Definir RM de forma compatível
ifeq ($(OS),Windows_NT)

RM = del /Q /F
FIX_PATH = $(subst /,\\,$1)
MKDIR = if not exist $(subst /,\\,$1) mkdir $(subst /,\\,$1)
else

RM = rm -rf
FIX_PATH = $1
MKDIR = mkdir -p $1
endif

# Compila recursivamente todos os arquivos .c dentro de `src`

.PHONY: all clean

all: $(TARGET)
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR):
	$(call MKDIR,$(OBJ_DIR))
$(BIN_DIR):
	$(call MKDIR,$(BIN_DIR))
clean:
	$(RM) $(OBJ_DIR) $(BIN_DIR) $(TARGET)
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Para compilar usando o Makfile:
# make: compila o projeto e gera o executável
# make clean: remove os arquivos objeto e o executável gerados
# make debug: compila o projeto com as flags de depuração e sanitização
