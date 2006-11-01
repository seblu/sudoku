bin_PROGRAMS=sudoku

sudoku_SOURCES=	src/sudoku.hh			\
		src/sudoku.cc			\
		src/grid.cc			\
		src/block.hxx

CLEANFILES= *~ '\#*'

.PHONY: tar re

tar: distcheck

re: clean all
