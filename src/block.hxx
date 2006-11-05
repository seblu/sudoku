#ifndef BLOCK_HXX_
# define BLOCK_HXX_

inline Block::Block(int value = 0) {
  value_ = value;
  for (int i = 0; i < GRID_SIDE; ++i)
    forbidden_[i] = false;
}

inline bool Block::is_forbidden(int value) const {
  if (!(value > 0 && value <= GRID_SIDE))
    throw std::string("Invalid is_forbidden value");
  return forbidden_[value - 1];
}

inline bool Block::is_set() const {
  return value_ != 0;
}

inline int Block::get() const
{
  return value_;
}

inline void Block::set(int val)
{
  if (!(val > 0 && val <= GRID_SIDE))
    throw std::string("Invalid value");
  if (value_ != 0)
    throw std::string("Value already set");
  if (forbidden_[val - 1])
    throw std::string("Set a forbidden value");
  value_ = val;
  for (register int i = 0; i < GRID_SIDE; ++i)
    forbidden_[i] = true;
}

inline void Block::forbid(int val) {
  if (!(val > 0 && val <= GRID_SIDE))
    throw std::string("Invalid forbid value");
  if (value_ != 0)
    throw std::string("Try to forbid whereas case is set");
  forbidden_[val - 1] = true;
  if (forbidden_count() == GRID_SIDE)
    throw std::string("All possibilities are forbidden.");
}

inline std::ostream &operator<<(std::ostream &stream, const Block &blk) {
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
