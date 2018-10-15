CXX = g++
CPPFLAGS = -g -Wall
STD_11 = -std=c++11

all: hypercube

hypercube: hypercube.cpp

	$(CXX) $(CPPFLAGS) $(STD_11) $^ -o $@


.PHONY: clean
clean:
	rm -rf hypercube
