#ifndef UTIL_H
#define UTIL_H

namespace util {
  std::string formatHex(uint32_t value, unsigned int minWidth = 0, bool xPrefix = true);
}

#endif