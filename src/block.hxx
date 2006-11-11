#ifndef BLOCK_HXX_
# define BLOCK_HXX_

inline Block::Block(int value = 0) {
  value_ = value;
  for (int i = 0; i < GRID_SIDE; ++i)
    forbidden_[i] = false;
}

inline bool Block::is_forbidden(int value) const throw (Error) {
  if (!(value > 0 && value <= GRID_SIDE))
    throw Error("Invalid \"is_forbidden\" value.");
  return forbidden_[value - 1];
}

inline bool Block::is_set() const {
  return value_ != 0;
}

inline int Block::get() const
{
  return value_;
}

inline void Block::set(int val) throw (Error)
{
  if (!(val > 0 && val <= GRID_SIDE))
    throw Error("Try to set an invalid value.", EXIT_INV_VAL);
  if (value_ != 0)
    throw Error("Try to set an already set value.");
  if (forbidden_[val - 1])
    throw Error("Try to set a forbidden value.");
  value_ = val;
  for (register int i = 0; i < GRID_SIDE; ++i)
    forbidden_[i] = true;
}

inline void Block::forbid(int val) throw (Error) {
  if (!(val > 0 && val <= GRID_SIDE))
    throw Error("Invalid forbid value.", EXIT_INV_VAL);
  if (value_ == val)
    throw Error("Try to forbid whereas case is set.", EXIT_INV_GRID);
  if (value_ != 0)
    return;
  forbidden_[val - 1] = true;
  if (forbidden_count() == GRID_SIDE)
    throw Error("All possibilities are forbidden.", EXIT_INV_GRID);
}

inline ostream &operator<<(ostream &stream, const Block &blk) {
  if (blk.value_ == 0)
    stream << " ";
  else
    stream << blk.value_;
  return stream;
}

inline int Block::forbidden_count() const
{
  int count = 0;

  for (int i = 0; i < GRID_SIDE; ++i)
    if (forbidden_[i])
      ++count;
  return count;
}

#endif
