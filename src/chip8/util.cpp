#include <string>
#include <cstdint>
#include <algorithm>

#include "util.h"

std::string util::formatHex(uint32_t value, unsigned int minWidth, bool xPrefix) {
  std::string formatted;
  char digits[] = "0123456789ABCDEF";

  do {
    int digit = value % 16;
    value = value / 16;
    formatted.push_back(digits[digit]);
  } while (value > 0);

  while (formatted.size() < minWidth) {
    formatted.push_back('0');
  }

  if (xPrefix) {
    formatted.append("x0");
  }

  if (formatted.size() > 1) {
    std::reverse(formatted.begin(), formatted.end());
  }

  return formatted;
}