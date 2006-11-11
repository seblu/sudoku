#include <stdlib.h>
#include <unistd.h>
#include "sudoku.hh"

inline static void usage();

int main(int argc, char *argv[])
{
  int opt;
  char *cmpfile = 0;

  while ((opt = getopt(argc, argv, "hvxc:")) != -1)
    switch (opt) {
    case 'v': Grid::verbose = true; break;
    case 'x': Grid::advprint = true; break;
    case 'c': cmpfile = optarg; break;
    case 'h':
    default: usage();
    }
  if (!argv[optind])
    usage();
  Grid *grid = new Grid();
  try {
    grid->load(argv[optind]);
    if (cmpfile) {
      Grid *grid2 = new Grid();
      grid2->load(cmpfile);
      if (*grid == *grid2) {
	if (Grid::verbose)
	  std::cout << "Grids are the same." << std::endl;
	return EXIT_FOUND;
      }
      if (Grid::verbose)
	std::cout << "Grids are differents." << std::endl;
	return EXIT_NOTFOUND;
    }
    if (Grid::verbose) {
      std::cout << "Loaded grid:" << std::endl;
      grid->print();
    }
    grid->solve();
    grid->print();
  }
  catch (const Error &e) {
    std::cerr << e.message() << std::endl;
    if (Grid::verbose)
      grid->print();
    return e.code();
  }
  sstream info;
  if (grid->check(info)) {
    if (Grid::verbose)
      std::cout << "Solution found." << std::endl;
    return EXIT_FOUND;
  }
  if (Grid::verbose) {
    std::cout << "Checking: " << info.str() << std::endl;
    std::cout << "No suitable solution found." << std::endl;
  }
  return EXIT_NOTFOUND;
}

inline static void usage()
{
  std::cerr << "Usage: sudoku [-vx] [-c file2] file" << std::endl;
  std::cerr << "Options:   -h: Print usage" << std::endl;
  std::cerr << "           -v: Verbose mode" << std::endl;
  std::cerr << "           -x: eXtended print mode" << std::endl;
  std::cerr << "           -c file2: compare file and file2" << std::endl;
  exit(EXIT_USAGE);
}
