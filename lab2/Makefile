CC=gcc
CFLAGS=-c -Wall
SOURCES=shell.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=shell

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *o $(EXECUTABLE)
