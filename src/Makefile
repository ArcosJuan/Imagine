.PHONY: all clean

BIN = main loader

CPP = main.cpp filters.cpp ppm.cpp loader.cpp helper.cpp
OBJ = $(CPP:.cpp=.o)
SRC = $(CPP)

all: main loader

clean:
	rm -f $(BIN) $(OBJ)

loader: loader.o filters.o ppm.o
	g++ -pthread -o $@ $^

main: main.o filters.o ppm.o helper.o
	g++ -pthread -o $@ $^

%.o: %.cpp
	g++ -pthread -c -o $@ $<
