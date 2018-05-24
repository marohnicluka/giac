all: graphtheory.cc graphe.cc graphtheory.h graphe.h
	g++ -g -Wall -std=c++98 -pedantic -c graphe.cc graphtheory.cc
	g++ -g -Wall -std=c++98 -pedantic graphtheory.o graphe.o test.cc -lgiac -lgmp
