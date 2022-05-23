CC = gcc
MKDIR=mkdir -p
RM=rm -rf

INC_DIR=inc
SRC_DIR=src
OBJ_DIR=obj
BUILD_DIR=build

IFLAGS=-I inc
CFLAGS=-g

SRC_FILES=$(wildcard src/*.c)
OBJ_FILES:=$(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.c, %.o, $(SRC_FILES)))

EXE=main

.PHONY: all clean

# all:
# 	echo $(OBJ_FILES)
all: $(BUILD_DIR)/$(EXE)

$(BUILD_DIR)/$(EXE) : $(OBJ_FILES)
	$(MKDIR) $(BUILD_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)/* $(BUILD_DIR)/*