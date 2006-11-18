#include "sudoku.hh"

bool Grid::verbose = false;
bool Grid::advprint = false;

void Grid::load(const char *filename) throw (Error)
{
  int val;
  std::fstream fs;

  assert(filename);
  if (Grid::verbose)
    std::cout << "Loading file: " << filename << "..." << std::endl;
  fs.open(filename, std::fstream::in);
  if (!fs) {
    Error e("Unable to open file: ", EXIT_LOADFAIL);
    e << filename;
    throw e;
  }
  try {
    for (int y = 0; y < GRID_SIDE; ++y)
      for (int x = 0; x < GRID_SIDE; ++x) {
	fs >> std::dec >> val;
	if (val < 0 || val > 9) {
	  Error e;
	  e << "Invalid value: " << val << " in file: " << filename;
	  throw e;
	}
	if (val > 0)
	  pose(x, y, val);
      }
  }
  catch (Error &e) {
    e.code(EXIT_LOADFAIL);
    e >> "'.\nReason: " >> filename >> "Load fail in file: '";
    e << "\nYour grid is incorrect!";
    fs.close();
    throw;
  }
  fs.close();
}

void Grid::pose(int x, int y, int v) throw (Error)
{
  Error e;

  if (x < 0 || x >= GRID_SIDE || y < 0 || y >= GRID_SIDE) {
    e << "Invalid position: " << x + 1 << ", " << y + 1;
    e.code(EXIT_INV_POS);
    throw e;
  }
  try {
    block_[x][y].set(v);
  }
  catch (const Error &er) {
    e << "Unable to pose " << v << " in (" << x + 1 << "," << y + 1 << "): "
      << er.message();
    e.code(er.code());
    throw e;
  }
  for (int i = 0; i < GRID_SIDE; ++i) {
    try {
      if (i != y)
	block_[x][i].forbid(v);
    }
    catch (Error &e) {
      e << " During seting case (" << x + 1 << "," << y + 1 << "), unable to forbid "
	<< v << " in (" << x + 1 << "," << i + 1 << ").";
      throw e;
    }
    try {
      if (i != x)
	block_[i][y].forbid(v);
    }
    catch (Error &e) {
      e << " During seting case (" << x + 1 << "," << y + 1 << "), unable to forbid "
	<< v << " in (" << i + 1 << "," << y + 1 << ").";
      throw e;
    }
  }
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      int a = (x / 3) * 3 + i;
      int b = (y / 3) * 3 + j;
      try {
	if (!(x == a && y == b))
	  block_[a][b].forbid(v);
      }
      catch (Error &e) {
	e << " During seting case (" << x + 1 << "," << y + 1 << "), unable to forbid "
	  << v << " in (" << a + 1 << "," << b + 1 << ").";
	throw e;
      }
    }
}

void Grid::solve()
{
  int posed;

  if (Grid::verbose)
    std::cout << "Resolving..." << std::endl;
  do {
    posed = 0;
    posed += solve_complete_block();
    posed += solve_square();
    posed += solve_line();
  }
  while (posed > 0);
}

int Grid::solve_complete_block()
{
  int posed = 0;

  for (int y = 0; y < GRID_SIDE; ++y)
    for (int x = 0; x < GRID_SIDE; ++x) {
      if (block_[x][y].is_set())
	continue;
      int count = 0, lastval;
      for (int i = 1; i <= GRID_SIDE; ++i)
	if (!block_[x][y].is_forbidden(i)) {
	  ++count;
	  lastval = i;
	  }
      if (count == 1) {
	pose(x, y, lastval);
	if (Grid::verbose)
	  std::cout << "Seting " << lastval << " to " << x << ","<< y << std::endl;
	++posed;
      }
    }
  return posed;
}

int Grid::solve_square()
{
  int posed = 0;

  for (int v = 1; v <= GRID_SIDE; ++v)
    for (int y = 0; y < GRID_SIDE; y += 3)
      for (int x = 0; x < GRID_SIDE; x += 3) {
	int v_count = 0;
	int last_x, last_y;
	for (int y2 = y; y2 < y + 3; ++y2)
	  for (int x2 = x; x2 < x + 3; ++x2)
	    if (!block_[x2][y2].is_forbidden(v)) {
	      last_x = x2;
	      last_y = y2;
	      ++v_count;
	    }
	std::cout << "block: (" << x << "," << y << "), val: " << v 
		  << ", v_count: " << v_count << std::endl;
	if (v_count == 1) {
	  std::cerr << "found " << v << " with solve_square in (" << last_x + 1
		    << "," << last_y + 1 << ")\n";
	  pose(last_x, last_y, v);
	  ++posed;
	}
      }
  return posed;
}

int Grid::solve_line()
{
  int posed = 0;

  for (int v = 1; v <= GRID_SIDE; ++v) //each val
    for (int i = 0; i < GRID_SIDE; ++i) { //each line and column
      int horizontal_count = 0, horizontal_x, horizontal_y;
      int vertical_count = 0, vertical_x, vertical_y;
      for (int j = 0; j < GRID_SIDE; ++j) {
	if (!block_[i][j].is_forbidden(v)) {
	  horizontal_x = i;
	  horizontal_y = j;
	  ++horizontal_count;
	}
	if (!block_[j][i].is_forbidden(v)) {
	  vertical_x = j;
	  vertical_y = i;
	  ++vertical_count;
	}
      }
      if (horizontal_count == 1) {
	std::cerr << "found " << v << " with solve_line_h in ("
		  << horizontal_x + 1
		  << "," << horizontal_y + 1 << ")" << std::endl;
	pose(horizontal_x, horizontal_y, v);
	++posed;
      }
      if (vertical_count == 1) {
	//dont pose the same number two times
	if (!(horizontal_x == vertical_x && horizontal_y == vertical_y)) {
	  std::cerr << "found " << v << " with solve_line_v in ("
		    << vertical_x + 1
		    << "," << vertical_y + 1 << ")" << std::endl;
	  pose(vertical_x, vertical_y, v);
	  ++posed;
	}
      }
    }
  return posed;
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
	      std::cout << "\033[1;32m" << j << "\033[0m";
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

bool Grid::check(sstream &info) const
{
  bool vertical_line[GRID_SIDE];
  bool horizontal_line[GRID_SIDE];
  int tmp;

  // check horizontals and verticales
  for (int i = 0; i < GRID_SIDE; ++i) {
    for (int j = 0; j < GRID_SIDE; ++j)
      horizontal_line[j] = vertical_line[j] = false;
    for (int j = 0; j < GRID_SIDE; ++j) {

      //horizontals (lines)

      tmp = block_[i][j].get() - 1;
      //check for empty block
      if (tmp == - 1) {
	info << "Grid is incomplete.";
	return false;
      }
      //check for crazy digits
      if (tmp < 0 || tmp > GRID_SIDE) {
	info << "Bad value " << tmp << " in (" << i + 1 << "," << j + 1
	     << ")." << std::endl;
	return false;
      }
      //check for more than one digit un line
      if (horizontal_line[tmp]) {
	info << tmp << " is already set in line " << i << ".";
	return false;
      }
      horizontal_line[tmp] = true;

      //verticals (columns)

      tmp = block_[j][i].get() - 1;
      //check for empty block
      if (tmp == - 1) {
	info << "Grid is incomplete.";
	return false;
      }
      //check for crazy digits
      if (tmp < 0 || tmp > GRID_SIDE) {
	info << "Bad value " << tmp << " in (" << j + 1 << "," << i + 1
	     << ")." << std::endl;
	return false;
      }
      //check for more than one digit un line
      if (vertical_line[tmp]) {
	info << tmp << " is already set in column " << i << ".";
	return false;
      }
      vertical_line[tmp] = true;
    }
    //check if lines and columns have all digits
    for (int j = 0; j < GRID_SIDE; ++j)
      if (!vertical_line[j]) {
	info << "Vertical line " << j + 1 << " is not full.";
	return false;
      }
      else if (!horizontal_line[j]) {
	info << "Horizontal line " << j + 1 << "is not full.";
	return false;
      }
  }
  // check nine blocks (use vertical_block)
  for (int i = 0; i < GRID_SIDE; i += 3)
    for (int j = 0; j < GRID_SIDE; j += 3) {
      for (int k = 0; k < GRID_SIDE; ++k)
	vertical_line[k] = false;
      for (int k = 0; k < 3; ++k)
	for (int l = 0; l < 3; ++l) {
	  tmp = block_[i + k][j + l].get() - 1;
	  assert(tmp >= 0 && tmp < GRID_SIDE);
	  if (vertical_line[tmp] == true)
	    return false;
	  vertical_line[tmp] = true;
	}
    }
  for (int j = 0; j < GRID_SIDE; ++j)
    if (!vertical_line[j])
      return false;
  return true;
}

bool Grid::operator==(const Grid &b) const
{
  for (int x = 0; x < GRID_SIDE; ++x)
    for (int y = 0; y < GRID_SIDE; ++y)
      if (this->block_[x][y].get() != b.block_[x][y].get())
	return false;
  return true;
}
