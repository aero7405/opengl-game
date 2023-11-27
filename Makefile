.default: all

CC := gcc
FLAGS := -Wall -Wextra -pedantic -ansi -Werror -g

LIBS := libs/libglfw3.a -lopengl32 -lgdi32
INCLUDE := -Iinclude

SRC := main.o sprite.o image.o
DEPS := gl.o

TARGET := Platformer

all: $(TARGET)

clean:
	del $(TARGET).* *.o

$(TARGET): $(SRC) $(DEPS)
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(DEPS) $(INCLUDE) $(LIBS)

%.o: %.c 
	$(CC) $(FLAGS) -c $< $(INCLUDE)

# this feels like a bad way of doing things but it works ¯\_(ツ)_/¯
%.o: deps/%.c
	$(CC) $(FLAGS) -c $< $(INCLUDE)