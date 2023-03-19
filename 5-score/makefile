CC = gcc
CFLAGS = -Wall -Werror -std=c11 -g -fsanitize=address -fsanitize=undefined -lrt
LDLIBS =

SRC = src/main.c src/file_io.c src/brackets_checker.c src/constants.c
OBJ = $(addprefix build/,$(notdir $(SRC:.c=.o)))

TARGET = bin/myprogram

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJ) -o $@ $(LDLIBS)

build/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/ bin/
    
.PHONY: all clean
