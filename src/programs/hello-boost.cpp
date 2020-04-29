#include <iostream>
#include <sstream>
#include <string>
#include <boost/format.hpp>

template <class T>
std::string toString(const T &val) {
  std::stringstream ss;
  ss << val;
  return ss.str();
}

int main() {
  using boost::format;
  std::string a = toString(boost::format("oi"));

  std::cout << a << "\n";
  return 0;
}