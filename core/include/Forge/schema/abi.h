#ifndef ABI_H
#define ABI_H

#include <string>

struct Abi {
  std::string compiler{};
  std::string stdlib{};
  std::string cxx_standard{};
  std::string c_standard{};
};

#endif // !ABI_H
