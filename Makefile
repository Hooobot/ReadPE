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

debug: DEBUG_FLAGS = -g -DDEBUG
debug: clean $(TARGET)

readpe: readpe.c
	$(CC) $(CFLAGS) -o $(TARGET) readpe.c

test: readpe
	./readpe

clean:
	rm -f $(TARGET) *.o
