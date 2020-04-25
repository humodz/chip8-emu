#include <algorithm>
#include <iostream>

class RuleOfFour {
private:
  size_t length;
  int *data;
public:
  friend void swap(RuleOfFour &lhs, RuleOfFour &rhs);

  RuleOfFour():
    length(0),
    data(nullptr) {
      std::cout << "Default constructor\n";
    }

  RuleOfFour(size_t length):
    length(length),
    data(new int[length]) {
      std::cout << "Constructor(length, data)\n";
    }

  // 1 - destructor
  ~RuleOfFour() {
    std::cout << "Destructor\n";
    delete[] data;
  }

  // 2 - copy constructor
  RuleOfFour(const RuleOfFour &that) {
    std::cout << "Copy constructor\n";
    length = that.length;
    data = new int[length];
    std::copy(that.data, that.data + length, data);
  }

  // 3 - copy assignment
  RuleOfFour &operator=(RuleOfFour that) {
    std::cout << "Copy assignment\n";
    swap(*this, that);
    return *this;
  }

  // 4 - move constructor
  RuleOfFour(RuleOfFour &&that) {
    std::cout << "Move constructor\n";
    swap(*this, that);
  }
  
  // 5 - move assignment
  // defaults to move constructor + copy assignment
};

// "and a half" - swap function
void swap(RuleOfFour &lhs, RuleOfFour &rhs) {
  std::cout << "Swap\n";
  using std::swap;
  swap(lhs.length, rhs.length);
  swap(lhs.data, rhs.data);
}

int main() {
  RuleOfFour foo;
  RuleOfFour bar;

  std::cout << "\n";
  bar = std::move(foo);
  std::cout << "\n";
  return 0;
}