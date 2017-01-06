build:
	clear
	gcc -std=c99 -O4 -pedantic main.c avl.c graph.c dijkstra-graph.c mymem.c queue.c set.c stack.c timer.c

run:
	clear
	./a.out
