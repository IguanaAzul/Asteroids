TARGET = main
LIBS = -lglut -lGL -lGLU -lm

all: default
    
default: main

main.o: main.c
	gcc -c main.c -o main.o
	
main: main.o
	gcc main.o -o main $(LIBS)

run: main
	./main

clean:
	-rm -f main
