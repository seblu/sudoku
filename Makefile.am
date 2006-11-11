bin_PROGRAMS=sudoku

sudoku_SOURCES=	src/sudoku.hh			\
		src/sudoku.cc			\
		src/grid.hxx			\
		src/grid.cc			\
		src/block.hxx			\
		src/error.cc

CLEANFILES= *~ '\#*'

.PHONY: tar re

tar: distcheck

re: clean all
