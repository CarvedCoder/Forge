#pragma once
#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>

namespace forge {
enum class PackageStatus { Resolved, Unresolved, None };

struct GitSource {
  std::string url{};
  std::string sha256{};
  std::string commit_hash{};
};

struct Package {
  std::string package_name{};
  std::string version{};
  GitSource source{};
  PackageStatus status{};
};

struct ResolvedPackage {
  std::string name{};
  GitSource source{};
};

} // namespace forge
#endif // !PACKAGE_H
