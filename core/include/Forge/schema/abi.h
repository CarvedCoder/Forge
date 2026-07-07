#pragma once
#ifndef ABI_H
#define ABI_H

#include <string>
namespace forge {
struct Abi {
  std::string compiler{};
  std::string stdlib{};
  std::string cxx_standard{};
  std::string c_standard{};
};

} // namespace forge

#endif // !ABI_H
