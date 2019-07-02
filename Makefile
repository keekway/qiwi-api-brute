CC = gcc
CCFLAGS = -std=c11 -lpthread
CCLIBCURL = $(shell pkg-config --cflags libcurl) $(shell pkg-config --libs libcurl)
CCARGS = $(CCFLAGS) $(CCLIBCURL)

SOURCES = sources/*.c

EXE = main

all:
	$(CC) $(CCARGS) $(SOURCES) -o $(EXE)

clean:
	rm -f $(EXE)
