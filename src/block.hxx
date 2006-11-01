#ifndef BLOCK_HXX_
# define BLOCK_HXX_

inline Block::Block()
{
  Block(0);
}

inline Block::Block(int value) {
  value_ = value;
  for (int i = 0; i < GRID_SIDE; ++i)
    forbidden_[i] = false;
}

inline bool Block::is_forbidden(int value) const {
  assert(value > 0 && value <= GRID_SIDE);
  return forbidden_[value];
}

inline int Block::value_get() const
{
  return value_;
}

inline void Block::set(int val)
{
  assert(val > 0 && val <= GRID_SIDE);
  value_ = val;
}

inline void Block::forbid(int val) {
  assert(val > 0 && val <= GRID_SIDE);
  if (val != value_)
    forbidden_[val - 1] = true;
}

inline std::ostream &operator<<(std::ostream &stream, const Block &blk) {
  if (blk.value_ == 0)
    stream << " ";
  else
    stream << blk.value_;
  return stream;
}

#endif
