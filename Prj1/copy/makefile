all:MyCopy ForkCopy PipeCopy 
clean:
	rm *.o
MyCopy:MyCopy.o
	gcc MyCopy.o -o MyCopy
MyCopy.o:MyCopy.c header.h
	gcc -c MyCopy.c 
ForkCopy:ForkCopy.o
	gcc ForkCopy.o -o ForkCopy
ForkCopy.o:ForkCopy.c header.h
	gcc -c ForkCopy.c 
PipeCopy:PipeCopy.o
	gcc PipeCopy.o -o PipeCopy
PipeCopy.o:PipeCopy.c header.h
	gcc -c PipeCopy.c 
