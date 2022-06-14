.POSIX:
CC = cc
CFLAGS = -pedantic -Wall -Os -s -Wno-deprecated-declarations -std=c99 -lX11
DEPS = fdw.h notif.h
OBJ = fdw.o notif.o sfnot.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sfnot: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -rf *.o
