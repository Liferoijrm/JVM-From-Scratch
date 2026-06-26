CC            ?= gcc
CFLAGS        ?= -Wall -Wextra -std=c99 \
                  -I src \
                  -I src/class_loader/loading \
                  -I src/class_loader/linking \
                  -I src/class_loader/initialization \
                  -I src/interpreter \
                  -I src/viewer \
                  -I src/utils \
                  -I src/runtime_data/method_area \
                  -I src/runtime_data/thread_data \
                  -I src/runtime_data/allocation
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

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(call MKDIR,$(OBJ_DIR))

$(BIN_DIR):
	$(call MKDIR,$(BIN_DIR))

clean:
	$(RM) $(OBJ_DIR) $(BIN_DIR)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all