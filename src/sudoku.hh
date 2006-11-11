
#ifndef SUDOKU_HH_
# define SUDOKU_HH_

#include <cassert>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>

const int GRID_SIDE = 9; //must be able to divise 3

enum {
  EXIT_FOUND = 0,
  EXIT_NOTFOUND = 1,
  EXIT_USAGE = 2,
  EXIT_LOADFAIL = 3,
  EXIT_INV_POS = 4,
  EXIT_INV_VAL = 5,
  EXIT_INV_GRID = 6,
  EXIT_OTHER = 7
};

typedef std::string string;
typedef std::stringstream sstream;
typedef std::ostream ostream;

class Error
{
public:
  Error(int i);
  Error(string s = "", int i = EXIT_OTHER);
  Error(const Error &e);
  void release() const;
  string message() const;
  int code() const;
  void code(int);
  Error &operator=(const Error &rhs);
  friend ostream &operator<<(ostream &o, const Error &e);
  friend Error &operator<<(Error &e, const string &s);
  friend Error &operator>>(Error &e, const string &s);
  friend Error &operator<<(Error &e, int val);
protected:
  sstream msg_;
  int val_;
};

class Block
{
public:
  inline Block(int val);
  inline bool is_forbidden(int val) const throw (Error);
  inline int forbidden_count() const;
  inline bool is_set() const;
  inline int get() const;
  inline void set(int val) throw (Error);
  inline void forbid(int val) throw (Error);
  inline friend ostream &operator<<(ostream &stream, const Block &blk);
protected:
  int value_;
  bool forbidden_[GRID_SIDE];
};

class Grid
{
public:
  void load(const char *filename) throw (Error);
  void print() const;
  inline void print_line() const;
  bool check(sstream &info) const;
  void solve();
  void pose(int x, int y, int v) throw (Error);
  bool operator==(const Grid &b) const;
  inline bool operator!=(const Grid &b) const;
  static bool verbose;
  static bool advprint;
protected:
  int solve_complete_block();
  int solve_square();
  int solve_line();
  Block block_[GRID_SIDE][GRID_SIDE];
};

#include "block.hxx"
#include "grid.hxx"

#endif
