# makefile to build main.exe. update main dependencies as needed
#
# changelog:
#
# 11-02-2018
#
# rewrote makefile, added dummy target and variables for targets
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

CC = gcc
CFLAGS = -Wall -g

# target names
# main target
MAIN_T = main
# stats target
STATS_T = stats
# strsea
STRSEA_T = strsea
# strh_table
STRH_TABLE_T = strh_table
# n_dirname
N_DIRNAME_T = n_dirname

# dummy target
dummy:

# creating the main executable; update dependencies depending on test
$(MAIN_T): $(MAIN_T).c stats.o # any other object files after main.c
	$(CC) $(CFLAGS) -o $(MAIN_T) $(MAIN_T).c stats.o # and any other object files after main.c

# stats package object file
$(STATS_T): $(STATS_T).c $(STATS_T).h
	$(CC) $(CFLAGS) -c $(STATS_T).c

# creates the strsea executable, which uses strsea.c, strh_table.h and strh_table.c
$(STRSEA_T): $(STRSEA_T).c strh_table.o
	$(CC) $(CFLAGS) -o $(STRSEA_T) $(STRSEA_T).c $(STRH_TABLE_T).o

# strh_table.* package object file (string hash table)
$(STRH_TABLE_T): $(STRH_TABLE_T).c $(STRH_TABLE_T).h
	$(CC) $(CFLAGS) -c $(STRH_TABLE_T).c

# n_dirname
$(N_DIRNAME_T): $(N_DIRNAME_T).c
	$(CC) $(CFLAGS) -o $(N_DIRNAME_T) $(N_DIRNAME_T).c

# clean autosave files from directory
clean:
	$(RM) -vf *~

# clean object files from directory too with autosave
clean_o:
	$(RM) -vf *.o *~

# clean object files and executables as well with autosave
clean_e:
	$(RM) -vf *.o *.exe *~

