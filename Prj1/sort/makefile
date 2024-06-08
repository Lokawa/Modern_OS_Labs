CC=gcc
CFLAGS=-I.
DEPS = header.h
OBJ = MergesortSingle.o MergesortMulti.o

all: MergesortSingle MergesortMulti
clean:
	rm *.o
MergesortSingle: MergesortSingle.o
	$(CC) -o $@ $^ $(CFLAGS)
MergesortMulti: MergesortMulti.o
	$(CC) -o $@ $^ $(CFLAGS)
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
