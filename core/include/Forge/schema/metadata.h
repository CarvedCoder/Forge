#pragma once
#ifndef METADATA_H
#define METADATA_H

#include <string>
namespace forge {
struct Metadata {
  std::string forge_version{};
  std::string generated_at{};
  std::string cmake_version{};
};

} // namespace forge

#endif // !METADATA_H
