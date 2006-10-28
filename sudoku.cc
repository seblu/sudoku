#include <stdlib.h>
#include "sudoku.hh"

bool Grid::verbose = false;

int main(int argc, char *argv[])
{
  if (argc != 2 && argc !=3) {
    std::cerr << "Usage: sudoku [-v] file" << std::endl;
    return 1;
  }
  if (argc == 3 && (argv[1] == "-v" || argv[2] == "-v"))
    Grid::verbose = true;
  Grid *grid = new Grid();
  grid->load(argv[Grid::verbose ? 1 : 2]);
  grid->print();
  grid->resolve();
  if (grid->verify()) {
    std::cout << "Solution found." << std::endl;
    grid->print();
  }
}

//
// GRID
//
int Grid::load(const char *filename)
{
  int val;
  std::fstream fs;

  std::cout << "coucou";
  try {
    fs.open(filename);
    std::cout << "sex";
  }
  catch (...) {
    std::cout << "sex";
  }
  std::cout << filename << std::endl;
  for (int i = 0; i < GRID_SIDE; ++i)
    for (int j = 0; j < GRID_SIDE; ++j) {
      fs >> std::dec >> val;
      std::cout << val << std::endl;
      //block_[i][j].set(val);
    }
  fs.close();
  return 0;
}

void Grid::resolve()
{
  
}

void Grid::print() const
{
  int i, j;

  for (i = 0; i < GRID_SIDE; ++i) {
    //show first line
    for (j = 0; j < GRID_SIDE; j += 3)
      if (j == GRID_SIDE - 3) std::cout << " --- --- ---" << std::endl;
      else std::cout << " --- --- ---  ";
    //show nine group separator
    if (i > 0 && i % 3 == 0) {
      std::cout << std::endl;
      for (j = 0; j < GRID_SIDE; j += 3)
	if (j == GRID_SIDE - 3) std::cout << " --- --- ---" << std::endl;
	else std::cout << " --- --- ---  ";
    }
    std::cout << "|";
    for (j = 0; j < GRID_SIDE; ++j) {
      if (j > 0 && j % 3 == 0) std::cout << " |";
      std::cout << " " << block_[i][j].get() << " |";
    }
    std::cout << std::endl;
  }
  for (j = 0; j < GRID_SIDE; j += 3)
    if (j == GRID_SIDE - 3) std::cout << " --- --- ---" << std::endl;
    else std::cout << " --- --- ---  ";
}

bool Grid::is_done() const
{
  for (int i = 0; i < GRID_SIDE; ++i)
    for (int j = 0; j < GRID_SIDE; ++j)
      if (block_[i][j].get() == 0)
	return false;
  return true;
}

bool Grid::verify() const
{
  bool vertical_line[GRID_SIDE];
  bool horizontal_line[GRID_SIDE];
  int tmp;

  // check horizontals and verticales
  for (int i = 0; i < GRID_SIDE; ++i) {
    for (int j = 0; j < GRID_SIDE; ++j)
      horizontal_line[j] = vertical_line[j] = false;
    for (int j = 0; j < GRID_SIDE; ++j) {
      //horizontals
      tmp = block_[i][j].get() - 1;
      if (tmp < 0 || horizontal_line[tmp])
	return false;
      horizontal_line[tmp] = true;
      //verticals
      tmp = block_[j][i].get() - 1;
      if (tmp < 0 || vertical_line[tmp] == true)
	return false;
      vertical_line[tmp] = true;
    }
    for (int j = 0; j < GRID_SIDE; ++j)
      if (!vertical_line[j] || !horizontal_line[j])
	return false;
  }
  // check nine blocks (use vertical_block)
  for (int j = 0; j < GRID_SIDE; ++j)
    vertical_line[j] = false;
  for (int i = 0; i < GRID_SIDE; i += 3)
    for (int j = 0; j < GRID_SIDE; j += 3)
      for (int k = 0; k < 3; ++k)
	for (int l = 0; l < 3; ++l) {
	  tmp = block_[i + k][j + l].get() - 1;
	  if (tmp < 0 || vertical_line[tmp] == true)
	    return false;
	  vertical_line[tmp] = true;
	}
    for (int j = 0; j < GRID_SIDE; ++j)
      if (!vertical_line[j])
	return false;
    return true;
}
