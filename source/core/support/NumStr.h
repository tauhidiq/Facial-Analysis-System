/**
  @file NumStr.h 
  Defines the header of a polymorphic class that behaves like a number and a string.
*/

#ifndef NUMSTR_H__
#define NUMSTR_H__

#include <string>

namespace kifas
{

/**
  A class that behaves like a number and a string. Used within the map container in the ConfigFileManager.
  @ingroup core
*/
class NumStr {
public:
  NumStr() {};
  explicit NumStr(const std::string&);
  explicit NumStr(double);
  explicit NumStr(const char*);

  NumStr(const NumStr&);
  NumStr& operator=(NumStr const&);

  NumStr& operator=(double);
  NumStr& operator=(std::string const&);

public:
  operator std::string() const;
  operator double     () const;
private:
  std::string mValue;
};

}

#endif