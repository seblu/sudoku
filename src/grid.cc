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
  if (block_[x][y].is_set()) {
    std::cerr << "Double positionning: " << x << ", " << y << std::endl;
    exit(EXIT_INV_VAL);
  }
  try {
    block_[x][y].set(v);
  }
  catch (std::string s) {
    std::cerr << "Unable to pose " << v << " in (" << x << "," << y << "): "
	      << s << std::endl;
    throw 2;
  }
  for (int i = 0; i < GRID_SIDE; ++i) {
    try {
      block_[x][i].forbid(v);
    }
    catch (std::string s) {
      std::cerr << "Unable to forbid " << v << " in (" << x << "," << y << "): "
		<< s << std::endl;
    }
    try {
      block_[i][y].forbid(v);
    }
    catch (std::string s) {
      std::cerr << "Unable to forbid " << v << " in (" << x << "," << y << "): "
		<< s << std::endl;
    }
  }
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      {
      std::cout << "forbid x:"<< x << ", y:" << y << ",i:" <<i <<",j;" <<j <<std::endl;
      block_[(x / 3) * 3 + i][(y / 3) * 3 + j].forbid(v);

      }
}

void Grid::resolve()
{
  int posed;

  if (Grid::verbose)
    std::cout << "Resolving..." << std::endl;
  do {
    posed = 0;
    for (int y = 0; y < GRID_SIDE; ++y)
      for (int x = 0; x < GRID_SIDE; ++x) {
	if (block_[x][y].is_set())
	  continue;
	//check for complete block
	int count = 0, lastval;
	for (int i = 1; i <= GRID_SIDE; ++i)
	  if (!block_[x][y].is_forbidden(i)) {
	    ++count;
	    lastval = i;
	}
	if (count == 1) {
	  pose(x, y, lastval);
	  if (Grid::verbose)
	    std::cout << "posed " << lastval << " to " << x << ","<< y << std::endl;
	  ++posed;
	}
      }
  }
  while (posed > 0);
}

void Grid::print() const
{
  int x, y;

  for (y = 0; y < GRID_SIDE; ++y) {
    print_line();
    if (y > 0 && y % 3 == 0)
      print_line();
    if (Grid::advprint) {
      for (int i = 0; i < 3; ++i) {
	for (x = 0; x < GRID_SIDE; ++x) {
	  if (x > 0 && x % 3 == 0)
	    std::cout << "| ";
	  std::cout << "|";
	  for (int j = i * 3 + 1; j <= i * 3 + 3; ++j)
	    if (block_[x][y].get() == j)
	      std::cout << "\033[0;32m" << j << "\033[0m";
	    else if (block_[x][y].is_forbidden(j))
	      std::cout << "\033[0;31m" << j << "\033[0m";
	    else
	      std::cout << j;
	}
	std::cout <<  "|" << std::endl;
      }
    }
    else {
      std::cout << "|";
      for (x = 0; x < GRID_SIDE; ++x) {
	if (x > 0 && x % 3 == 0) std::cout << " |";
	std::cout << " " << block_[x][y] << " |";
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
  std::cout << "sex" << std::endl;
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
  std::cout << "city" << std::endl;
  for (int j = 0; j < GRID_SIDE; ++j)
    if (!vertical_line[j])
      return false;
  return true;
}
