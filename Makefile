CC=g++
CFLAGS= -c -g -Wall -std=c++20
LDFLAGS= -lm 
SOURCES=main.cpp general.cpp write_struct.cpp read_struct.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(EXECUTABLE)
	rm -rf *.o

$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS)  $< -o $@ $(LDFLAGS)