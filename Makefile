CXX=g++
CXXFLAGS=-Wall -std=c++14
LFLAGS =-Wall -lboost_program_options


fifo: fifo.o
	$(CXX)  fifo.o -o fifo $(LFLAGS)

.PHONY: clean
clean:
	rm fifo fifo.o
