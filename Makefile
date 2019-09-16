#
# Name:	Junxiang Wen
# Last Edit: 09/011/2019
# Topic:     Project 1: Giorgio's Discrete Event Simulator


project1: Project1.o
	gcc Project1.c -o project1
	./project1

.PHONY: clean  
clean:  
	rm -f *.o

