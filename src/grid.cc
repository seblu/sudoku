#include "sudoku.hh"

bool Grid::verbose = false;
bool Grid::advprint = false;

int Grid::load(const char *filename)
{
  int val;
  std::fstream fs;

  assert(filename);
  if (Grid::verbose)
    std::cout << "Loading file: " << filename << "..." << std::endl;
  fs.open(filename, std::fstream::in);
  if (!fs.is_open())
    return 0;
  for (int y = 0; y < GRID_SIDE; ++y)
    for (int x = 0; x < GRID_SIDE; ++x) {
      fs >> std::dec >> val;
      if (val < 0 || val > 9) {
	std::cerr << "Invalid value in file: " << val << std::endl;
	exit(EXIT_LOADFAIL);
      }
      if (val > 0)
	pose(x, y, val);
      //block_[i][j].set(val);
    }
  fs.close();
  return 1;
}

void Grid::pose(int x, int y, int v)
{
  if (x < 0 || x >= GRID_SIDE || y < 0 || y >= GRID_SIDE) {
    std::cerr << "Invalid position: " << x + 1 << ", " << y + 1 << std::endl;
    exit(EXIT_INV_POS);
  }
  if (v <= 0 || v > GRID_SIDE) {
    std::cerr << "Invalid value: " << v << std::endl;
    exit(EXIT_INV_VAL);
  }
  for (int i = 0; i < GRID_SIDE; ++i) {
    block_[x][i].forbid(v);
    block_[i][y].forbid(v);
  }
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      block_[(x / 3) * 3 + x][(y / 3) * 3 + y].forbid(v);
}

void Grid::resolve()
{
  if (Grid::verbose)
    std::cout << "Resolving..." << std::endl;
}

void Grid::print() const
{
  int i, j, k;

  for (i = 0; i < GRID_SIDE; ++i) {
    print_line();
    if (i > 0 && i % 3 == 0)
      print_line();
    if (Grid::advprint) {
      for (int l = 0; l < 3; ++l) {
	for (j = 0; j < GRID_SIDE; ++j) {
	  if (j > 0 && j % 3 == 0)
	    std::cout << "| ";
	  std::cout << "|";
	  for (k = l * 3 + 1; k <= l * 3 + 3; ++k)
	    if (block_[i][j].value_get() == k)
	      std::cout << "\033[0;32m" << k  << "\033[0m";
	    else if (block_[i][j].is_forbidden(k))
	      std::cout << "\033[0;31m" << k  << "\033[0m";
	    else
	      std::cout << k;
	}
	std::cout <<  "|" << std::endl;
      }
    }
    else {
      std::cout << "|";
      for (j = 0; j < GRID_SIDE; ++j) {
	if (j > 0 && j % 3 == 0) std::cout << " |";
	std::cout << " " << block_[i][j] << " |";
      }
      std::cout << std::endl;
    }
  }
  print_line();
}

bool Grid::is_done() const
{
  for (int i = 0; i < GRID_SIDE; ++i)
    for (int j = 0; j < GRID_SIDE; ++j)
      if (block_[i][j].value_get() == 0)
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
      tmp = block_[i][j].value_get() - 1;
      if (tmp < 0 || horizontal_line[tmp])
	return false;
      horizontal_line[tmp] = true;
      //verticals
      tmp = block_[j][i].value_get() - 1;
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
	  tmp = block_[i + k][j + l].value_get() - 1;
	  if (tmp < 0 || vertical_line[tmp] == true)
	    return false;
	  vertical_line[tmp] = true;
	}
    for (int j = 0; j < GRID_SIDE; ++j)
      if (!vertical_line[j])
	return false;
    return true;
}
