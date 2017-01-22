#******************************************************************************
## Function name: Makefile
##
## DESCRIPTION:   A set of commands to compile all files in HW7a
##
## Parameters:    n/a
##
## Return values: n/a
##
##*****************************************************************************

# Declare the variables
CC = gcc

CFLAGS = -c

# This target will compile all the files
all: hello

# This target will compile all the files
hello: main.o functions.o
	$(CC) -o HW7a main.o functions.o

# This target will compile main.o only
main.o: main.c
	$(CC) $(CFLAGS) main.c

# This target will compile main.o only
functions.o: functions.c
	$(CC) $(CFLAGS) functions.c

# This target will remove the executable and all object files
clean:
	rm -rf *o HW7a
