
#ifndef SUDOKU_HH_
# define SUDOKU_HH_

#include <cassert>
#include <ios>
#include <iostream>
#include <fstream>

const int GRID_SIDE = 9; //must be able to divise 3

enum {
  EXIT_FOUND = 0,
  EXIT_NOTFOUND = 1,
  EXIT_USAGE = 2,
  EXIT_LOADFAIL = 3,
  EXIT_INV_POS = 4,
  EXIT_INV_VAL = 5
};

class Block
{
public:
  inline Block();
  inline Block(int val);
  inline bool is_forbidden(int val) const;
  inline int value_get() const;
  inline void set(int val);
  inline void forbid(int val);
  inline friend std::ostream &operator<<(std::ostream &stream, const Block &blk);
private:
  int value_;
  bool forbidden_[GRID_SIDE];
};

class Grid
{
public:
  int load(const char *filename);
  void print() const;
  inline void print_line() const;
  bool verify() const;
  bool is_done() const;
  void resolve();
  void pose(int x, int y, int v);
  static bool verbose;
  static bool advprint;
private:
  Block block_[GRID_SIDE][GRID_SIDE];
};

void Grid::print_line() const
{
  for (int j = 0; j < GRID_SIDE; ++j) {
    if (j % 3 == 0 && j > 0 && j < GRID_SIDE)
      std::cout << "  ";
    std::cout << " ---";
    if (j == GRID_SIDE - 1)
      std::cout << std::endl;
  }
}

#include "block.hxx"

#endif
