INCLUDE_DIR := include
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE = $(BIN_DIR)/benchmark.elf

SRC_C = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(SRC_C:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = $$HOME/x-tools/m68k-unknown-elf/bin/m68k-unknown-elf-gcc
LD = $$HOME/x-tools/m68k-unknown-elf/bin/m68k-unknown-elf-ld
CFLAGS = -Wall -nostdlib -nodefaultlibs -m68000
LDFLAGS = --defsym=_start=main -Ttext=0x10000 -Tdata=0x3000 -Tbss=0x4000 --section-start=.rodata=0x5000
CPPFLAGS = -I$(INCLUDE_DIR) -MMD -MP

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

-include $(OBJ:.o=.d)

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)