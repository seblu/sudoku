
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
  Block() { Block(0); }
  Block(int value) {
    value_ = value;
    for (int i = 0; i < GRID_SIDE; ++i)
      forbidden_[i] = false;
  }
  bool is_forbidden(int value) const {
    assert(value > 0 && value <= GRID_SIDE);
    return forbidden_[value];
  }
  int value_get() const { return value_; }
  bool is_forbid(int v) const {
    assert(v > 0 && v <= GRID_SIDE);
    return forbidden_[v - 1];
  }
  void init(int val) { assert(val >= 0 && val <= GRID_SIDE); value_ = val; }
  void set(int val) { assert(val > 0 && val <= GRID_SIDE); value_ = val; }
  void forbid(int val) {
    assert(val > 0 && val <= GRID_SIDE);
    if (val != value_)
      forbidden_[val] = true;
  }
  friend std::ostream &operator<<(std::ostream &stream, const Block &blk);
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

#endif
