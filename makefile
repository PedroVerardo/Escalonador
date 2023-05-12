all:
	gcc -o round_robin round_robin.c
	gcc -o real_time real_time.c
	gcc -o main interpreter.c
	./main
queue:
	gcc -Wall -o queue Queue.c
	./queue