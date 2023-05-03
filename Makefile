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

# Compiling components of readpe
doshdr.o: doshdr.c doshdr.h
	$(CC) $(CFLAGS) -c doshdr.c -o doshdr.o

coffhdr.o: coffhdr.c coffhdr.h
	$(CC) $(CFLAGS) -c coffhdr.c -o coffhdr.o

optionhdr.o: optionhdr.c optionhdr.h
	$(CC) $(CFLAGS) -c optionhdr.c -o optionhdr.o

sectionhdr.o: sectionhdr.c sectionhdr.h
	$(CC) $(CFLAGS) -c sectionhdr.c -o sectionhdr.o

readpe.o: readpe.c doshdr.h coffhdr.h optionhdr.h sectionhdr.h
	$(CC) $(CFLAGS) -c readpe.c -o readpe.o

readpe: readpe.o doshdr.o coffhdr.o optionhdr.o sectionhdr.o
	$(CC) $(CFLAGS) -o $(TARGET) readpe.o doshdr.o coffhdr.o optionhdr.o sectionhdr.o

# Testing within a directory
test: readpe
	./readpe testing/catnap32.exe
	./readpe testing/catnap64.exe

clean:
	rm -f $(TARGET) *.o
