all:
	gcc -Wall -o main main.c interpreter2.c scheduler.c ordered_list.c queue.c
	./main
L:
	gcc -o p1 p1.c
	gcc -o p2 p2.c
	gcc -o p3 p3.c
	gcc -o p4 p4.c
	gcc -o p5 p5.c
	gcc -o p6 p6.c