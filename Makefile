CC=g++
CFLAGS=-std=c++17 -lssl -lcrypto

OBJ=main.o ParseParams.o pop3.o
DEPS=pop3.h ParseParams.h

all:popcl

%.o: %.c $(DEPS)
	@(CC) -c -o $@ $< $(CFLAGS)
popcl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
clean:
	-rm $(OBJ)
	-rm popcl

