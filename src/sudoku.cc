#include <stdlib.h>
#include <unistd.h>
#include "sudoku.hh"

inline static void usage();

int main(int argc, char *argv[])
{
  int opt;

  if (argc != 2 && argc !=3)
    usage();
  while ((opt = getopt(argc, argv, "vx")) != -1)
    switch (opt) {
    case 'v': Grid::verbose = true; break;
    case 'x': Grid::advprint = true; break;
    default: usage();
    }
  Grid *grid = new Grid();
  if (!grid->load(argv[optind])) {
    std::cerr << "Loading failed !" << std::endl;
    return EXIT_LOADFAIL;
  }
  if (Grid::verbose) {
    std::cout << "Loaded grid:" << std::endl;
    grid->print();
  }
  grid->resolve();
  grid->print();
  if (grid->verify()) {
    if (Grid::verbose)
      std::cout << "Solution found." << std::endl;
    return EXIT_FOUND;
  }
  std::cout << "No suitable solution found." << std::endl;
  return EXIT_NOTFOUND;
}

inline static void usage()
{
  std::cerr << "Usage: sudoku [-vx] file" << std::endl;
  std::cerr << "Options:   -v: Verbose mode" << std::endl;
  std::cerr << "           -x: eXtended print mode" << std::endl;
  exit(EXIT_USAGE);
}
