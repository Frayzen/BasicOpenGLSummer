CC=g++
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast -fsanitize=address
LDFLAGS=  -fsanitize=address -lGL -lGLEW -lglfw
OUT= out
SRC_DIR=./
SRCS = $(shell find $(SRC_DIR) -name '*.cc' -a ! -path '*/tests/*')
OBJS = $(SRCS:.cc=.o)



all: $(OBJS)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o')

.PHONY: clean all

