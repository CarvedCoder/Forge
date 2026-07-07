#ifndef METADATA_H
#define METADATA_H

#include <string>

struct Metadata {
  std::string forge_version{};
  std::string generated_at{};
  std::string cmake_version{};
};

#endif // !METADATA_H
