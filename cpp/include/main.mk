CXX = g++
CXXFLAGS ?= -g -Wall -ansi -pedantic
SRCMODULES ?= 
OBJMODULES = $(SRCMODULES:.c=.o)

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run: clean main
	./main

main: main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o main