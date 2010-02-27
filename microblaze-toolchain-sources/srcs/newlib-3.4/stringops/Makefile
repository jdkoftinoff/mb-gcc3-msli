CC = mb-gcc
AS = mb-as
AR = mb-ar
RANLIB = mb-ranlib

all: strcpy.S strcmp.S strlen.S
	${CC} -c strcpy.S -o strcpy.o
	${CC} -c strcmp.S -o strcmp.o
	${CC} -c strlen.S -o strlen.o

clean: 
	rm -rf *.o
