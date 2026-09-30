CC     = gcc
CFLAGS = -Wall -O2 -Isrc
FONTES = $(wildcard src/*.c)

all: bin/pi3b

bin/pi3b: $(FONTES)
	mkdir -p bin
	$(CC) $(CFLAGS) $(FONTES) -o bin/pi3b

clean:
	rm -rf bin

.PHONY: all clean