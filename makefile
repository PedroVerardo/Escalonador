all:
	gcc -o main interpreter.c
	./main
c:
	gcc -Wall -o esc escalonador.c Queue.c
	./esc
L:
	gcc -o p1 p1.c
	gcc -o p2 p2.c
	gcc -o p3 p3.c