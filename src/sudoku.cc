#include <stdlib.h>
#include "sudoku.hh"

int main(int argc, char *argv[])
{
  int found;

  if (argc != 2 && argc !=3) {
    std::cerr << "Usage: sudoku [-v] file" << std::endl;
    return EXIT_USAGE;
  }
  if (argc == 3 && argv[1] == (std::string) "-v")
    Grid::verbose = true;
  if (argc == 3 && argv[1] == (std::string) "-p")
    Grid::advprint = true;
  Grid *grid = new Grid();
  if (!grid->load(argv[Grid::verbose || Grid::advprint ? 2 : 1])) {
    std::cerr << "Loading failed !" << std::endl;
    return EXIT_LOADFAIL;
  }
  if (Grid::verbose) {
    std::cout << "Loaded grid:" << std::endl;
    grid->print();
  }
  grid->resolve();
  found = grid->verify() ? EXIT_FOUND : EXIT_NOTFOUND;
  if (found == EXIT_FOUND && Grid::verbose > 0)
    std::cout << "Solution found." << std::endl;
  grid->print();
  return found;
}

std::ostream &operator<<(std::ostream &stream, const Block &blk) {
  if (blk.value_ == 0)
    stream << " ";
  else
    stream << blk.value_;
  return stream;
}
