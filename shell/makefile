CC=gcc
CFLAGS=-I.
DEPS=header.h
OBJ=shell.o

all:shell
clean:
	rm *.o
shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
