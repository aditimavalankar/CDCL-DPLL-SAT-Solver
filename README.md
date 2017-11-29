CSE 291E: Programming Assignment 1

CDCL implementation. The algorithm runs for 30s and if a solution is not found, it returns TIMEOUT. If a satisfying assignment is found, it prints 'SAT' along with the assignment, otherwise it prints 'UNSAT'.

Instructions to run the code:

1. Type 'make'. You may see a warning for C++11, that can be ignored for now.
2. An executable named 'sat' will be created. Run sat by typing './sat relative-location-of-cnf-file'

Note:

1. Type 'make clean' to remove .o files.
2. There may be satisfying assignments, but it is possible they are not found within the given time limit.

Results:

The results can be found in the file 'output.txt'.
Conflict analysis can be found in the file 'conflict_analysis.txt'
