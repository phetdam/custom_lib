# makefile to build main.exe. update main dependencies as needed
#
# Changelog:
#
# 11-03-2018
#
# renamed main target and associated variables to custom_lib_test. added a list
# of dependencies for the target and abbreviated target name for custom_lib_test
#
# 11-02-2018
#
# rewrote makefile, added dummy target and variables for targets. removed target
# for n_dirname, and added target for d_array
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

# stats target
STATS_T = stats
# strsea
STRSEA_T = strsea
# strh_table
STRH_TABLE_T = strh_table
# d_array target
D_ARRAY_T = d_array

# custom_lib_test target
CUSTOM_LIB_TEST_T = custom_lib_test
# dependencies for custom_lib_test
CUSTOM_LIB_TEST_DEPS = $(D_ARRAY_T).o

# dummy target
dummy:

# shorter way to refer to target $(CUSTOM_LIB_TEST_T)
test: $(CUSTOM_LIB_TEST_T)

# creating the main test driver; update dependencies depending on test
$(CUSTOM_LIB_TEST_T): $(CUSTOM_LIB_TEST_T).c $(CUSTOM_LIB_TEST_DEPS)
	$(CC) $(CFLAGS) -o $(CUSTOM_LIB_TEST_T) $(CUSTOM_LIB_TEST_T).c $(CUSTOM_LIB_TEST_DEPS)

# stats package object file
$(STATS_T): $(STATS_T).c $(STATS_T).h
	$(CC) $(CFLAGS) -c $(STATS_T).c

# creates the strsea executable, which uses strsea.c, strh_table.h and strh_table.c
$(STRSEA_T): $(STRSEA_T).c $(STRH_TABLE_T).o
	$(CC) $(CFLAGS) -o $(STRSEA_T) $(STRSEA_T).c $(STRH_TABLE_T).o

# strh_table.* package object file (string hash table)
$(STRH_TABLE_T): $(STRH_TABLE_T).c $(STRH_TABLE_T).h
	$(CC) $(CFLAGS) -c $(STRH_TABLE_T).c

# d_array package object file
$(D_ARRAY_T): $(D_ARRAY_T).c $(D_ARRAY_T).h
	$(CC) $(CFLAGS) -c $(D_ARRAY_T).c

# clean autosave files from directory
clean:
	$(RM) -vf *~

# clean object files from directory too with autosave
clean_o:
	$(RM) -vf *.o *~

# clean object files and executables as well with autosave
clean_e:
	$(RM) -vf *.o *.exe *~

