CXX=g++
INC=-I ./include
CFLAGS=-std=c++14 -W -Wall -pedantic $(INC)

main: main.cpp ./src/*
	$(CXX) $(CFLAGS) -o $@ $^

debug: main.cpp ./src/*
	$(CXX) $(CFLAGS) -g -o $@ $^

clean:
	rm -f main debug *.png *.txt ./src/saida.txt ./include/saida.txt

.PHONY: clean