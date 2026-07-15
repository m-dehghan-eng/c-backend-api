
CC = gcc

CFLAGS = -Wall -Wextra -O2 -g -DFIO_WEBSOCKET=0 \
  -I./lib/facil/lib \
  -I./lib/facil/lib/facil \
  -I./lib/facil/lib/facil/core \
  -I./lib/facil/lib/facil/http \
  -I./lib/facil/lib/facil/fiobj \
  -I./lib/facil/lib/facil/redis \
  -I./src \
  -I./src/Controllers \
  -I./src/db

LDFLAGS = -L./lib/facil/build -lfacil.io -lpthread -ldl -lrt -lm

SRC = $(wildcard src/*.c) \
      $(wildcard src/Controllers/*.c) \
      $(wildcard src/db/*.c) \
      $(wildcard src/data/*.c)


OBJ = $(SRC:.c=.o)

TARGET = weapi

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
