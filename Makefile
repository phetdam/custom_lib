# makefile to build main.exe. update main dependencies as needed
#
# changelog:
#
# 08-30-2018
#
# initial edit
CC=gcc
CFLAGS=-Wall -g
# creating the main executable; update dependencies depending on test
main: main.c stats.o # any other object files after main.c
	$(CC) $(CFLAGS) -o main.exe main.c stats.o # and any other object files after main.c

# stats package object file
stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c

# clean directory of object files and autosave files
clean:
	$(RM) -vf *.o *~

