# makefile to build main.exe. update main dependencies as needed
#
# changelog:
#
# 10-14-2018
#
# added target for n_dirname
#
# 09-20-2018
#
# added targets for strh_table.* and strsea; removed extension for stats.o target
# (now just plain stats)
#
# 08-30-2018
#
# initial edit

CC=gcc
CFLAGS=-Wall -g

# creating the main executable; update dependencies depending on test
main: main.c stats.o # any other object files after main.c
	$(CC) $(CFLAGS) -o main main.c stats.o # and any other object files after main.c

# stats package object file
stats: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c

# creates the strsea executable, which uses strsea.c, strh_table.h and strh_table.c
strsea: strsea.c strh_table.o
	$(CC) $(CFLAGS) -o strsea strsea.c strh_table.o

# strh_table.* package object file (string hash table)
strh_table: strh_table.c strh_table.h
	$(CC) $(CFLAGS) -c strh_table.c

# n_dirname
n_dirname: n_dirname.c
	$(CC) $(CFLAGS) -o n_dirname n_dirname.c

# clean directory of object files and autosave files
clean:
	$(RM) -vf *.o *~

