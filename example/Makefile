# Joe Jevnik

CC=gcc
CFLAGS=-Wall
LIBS=-lc_nxt -lbluetooth
INCLUDE=
SOURCES=robot.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=robot

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -L. $(LIBS) -o $@

.c.o:
	$(CC) -c $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE) $(OBJECTS)
