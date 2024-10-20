CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c18

SRC_DIR = src
OBJ_DIR = build
SRCS = $(shell find $(SRC_DIR) -name '*.c') 
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = o


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)


.PHONY: clean

release:
	python3 build.py