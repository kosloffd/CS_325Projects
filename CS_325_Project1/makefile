
# ===========================================================================
# Title       : CS325 Project 1 Group 30 - Analysis of Algorithms
# Members	  : John Fitzpatrick, Dylan York, Jarrad Favre
# Date	      : 04/17/2016
# Subject     : Assignment 0 makefile
# Description : Simple makefile to build our c program to text algorithms
# 				Use 'make all' to build the program, and 'prog' to run
# ===========================================================================


all: prog

prog: utility.o main.o
	gcc -g -Wall -std=c99 -o prog four_algorithms.o utility.o

four_algorithms.o: four_algorithms.c
	gcc -g -Wall -std=c99 -c main.c

utility.o: utility.c utility.h
	gcc -g -Wall -std=c99 -c utility.c

clean:
	rm utility.o
	rm four_algorithms.o
	rm prog
