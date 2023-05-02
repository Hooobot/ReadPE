###############################################################################
###               University of Hawaii, College of Engineering
###               Lab 6 - Readpe - SRE - Spring 2023
###
### Build and test a Portable Executor Reader program
###
### @see     https://www.gnu.org/software/make/manual/make.html
###
### @file    Makefile
### @author  Hubert Liang <hubertl@hawaii.edu>
###############################################################################


TARGET = readpe

all: $(TARGET)

CC     = gcc
CFLAGS = -Wall -Wextra $(DEBUG_FLAGS)

# No debug currently implemented
debug: DEBUG_FLAGS = -g -DDEBUG
debug: clean $(TARGET)

readpe.o: readpe.c readpe.h
	$(CC) $(CFLAGS) -c  readpe.c -o readpe.o

readpe: readpe.o
	$(CC) $(CFLAGS) -o $(TARGET) readpe.o

test: readpe
	./readpe testing/catnap32.exe
	./readpe testing/catnap64.exe

clean:
	rm -f $(TARGET) *.o
