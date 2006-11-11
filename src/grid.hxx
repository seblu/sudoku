#ifndef GRID_HXX_
# define GRID_HXX_

inline void Grid::print_line() const
{
  for (int j = 0; j < GRID_SIDE; ++j) {
    if (j % 3 == 0 && j > 0 && j < GRID_SIDE)
      std::cout << "  ";
    std::cout << " ---";
    if (j == GRID_SIDE - 1)
      std::cout << std::endl;
  }
}

inline bool Grid::operator!=(const Grid &b) const
{
  return !(*this == b);
}

#endif
