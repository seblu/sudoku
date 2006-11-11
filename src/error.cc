#include "sudoku.hh"

Error::Error(int v)
{
  val_ = v;
}

Error::Error(const string s, int i)
{
  val_ = i;
  msg_ << s;
}

Error::Error(const Error &e)
{
  val_ = e.val_;
  msg_ << e.msg_.str();
}

string Error::message() const
{
  return msg_.str();
}

int Error::code() const
{
  return val_;
}

void Error::code(int v)
{
  val_ = v;
}

Error &Error::operator=(const Error &rhs)
{
  val_ = rhs.val_;
  msg_.str("");
  msg_ << rhs.msg_.str();
  return *this;
}

ostream &operator<<(ostream &o, const Error &e)
{
  o << e.msg_.str();
  return o;
}

Error &operator<<(Error &e, int val)
{
  e.msg_ << val;
  return e;
}

Error &operator<<(Error &e, const string &s)
{
  e.msg_ << s;
  return e;
}

Error &operator>>(Error &e, const string &s)
{
  string buf = s + e.msg_.str();

  e.msg_.str("");
  e.msg_ << buf;
  return e;
}
