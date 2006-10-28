
#ifndef SUDOKU_HH_
# define SUDOKU_HH_

const int GRID_SIDE = 9; //must be able to divise 3

#include <cassert>
#include <ios>
#include <iostream>
#include <fstream>

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
  int get() const { return value_; }
  void set(int val) { assert(val > 0 && val <= GRID_SIDE); value_ = val; }
  void forbid(int val) {
    assert(val > 0 && val <= GRID_SIDE);
    forbidden_[val] = true;
  }
private:
  int value_;
  bool forbidden_[GRID_SIDE];
};

class Grid
{
public:
  int load(const char *filename);
  void print() const;
  bool verify() const;
  bool is_done() const;
  void resolve();
  static bool verbose;
private:
 
  Block block_[GRID_SIDE][GRID_SIDE];
};

#endif
