all: graphtheory.cc
	g++ -g -Wall -pedantic -c graphtheory.cc
	g++ -g -Wall -pedantic graphtheory.o test.cc -lgiac -lgmp
