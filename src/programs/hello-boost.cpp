#include <iostream>
#include <boost/format.hpp>

#define FMT(x) "[" #x "]"

int main() {
  using boost::format;
  using boost::io::group;

  int value = 0x1AB;
  int reg = 0xD;

  std::cout << FMT(123) << "\n";
  return 0;
}