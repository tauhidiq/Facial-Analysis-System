#include "NumStr.h"

#include <string>
#include <sstream>
#include <stdlib.h>
/**
  Constructs the NumStr from a string.
  @param value the string to construct the NumStr.
*/
kifas::NumStr::NumStr(std::string const& value) {
  mValue=value;
}

#include <iostream>

/**
  Constructs the NumStr from a array of chars.
  @param c the pointer to the array of chars to construct the NumStr.
*/
kifas::NumStr::NumStr(const char* c) {
  mValue=c;
}

/**
  Constructs the NumStr from a double.
  @param d the double to construct the NumStr.
*/
kifas::NumStr::NumStr(double d) {
  std::stringstream s;
  s<<d;
  mValue=s.str();
}

/**
  Copy constructor.
  @param other is another NumStr to copy from.
*/
kifas::NumStr::NumStr(kifas::NumStr const& other) {
  mValue=other.mValue;
}

/**
  Assigns the value of another NumStr into this one.
  @param other the NumStr that we are copying from.
  @return a reference of this new NumStr.
*/
kifas::NumStr& kifas::NumStr::operator=(kifas::NumStr const& other) {
  mValue=other.mValue;
  return *this;
}

/**
  Assign the number to our NumStr, convert it to string.
  @param i the number we are assiging.
  @return a reference of this NumStr.
*/
kifas::NumStr& kifas::NumStr::operator=(double i) {
  std::stringstream s;
  s << i;
  mValue = s.str();
  return *this;
}

/**
  Assign the string to our NumStr.
  @param s the string we are assigning.
  @return a reference to this NumStr.
*/
kifas::NumStr& kifas::NumStr::operator=(std::string const& s) {
  mValue=s;
  return *this;
}

/**
  The NumStr should behave like a string, so return the value as is.
  @return the string value of this NumStr.
*/
kifas::NumStr::operator std::string() const {
  return mValue;
}

/**
  The NumStr should behave like a number, so convert it to double.
  @return the double value stored in our string.
  @warning If the string is not a valid number, zero would be returned.
*/
kifas::NumStr::operator double() const {
  return atof(mValue.c_str());
}
