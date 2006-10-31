.PHONY: all clean distclean

all: sudoku

sudoku: sudoku.o grid.o
	g++ -Wall -Wextra -pedantic -ansi -g $^ -o $@

%.o:%.cc sudoku.hh
	g++ -Wall -Wextra -pedantic -ansi -g $< -o $@ -c

clean:
	rm -f *.o *~ \#*\#

distclean: clean
	rm -f sudoku