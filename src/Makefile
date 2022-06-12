.PHONY: all clean

BIN = imagine

CPP = imagine.cpp filters.cpp ppm.cpp helper.cpp
OBJ = $(CPP:.cpp=.o)
SRC = $(CPP)

all: imagine

clean:
	rm -f $(BIN) $(OBJ)

imagine: imagine.o filters.o ppm.o helper.o
	g++ -pthread -o $@ $^

%.o: %.cpp
	g++ -pthread -c -o $@ $<
