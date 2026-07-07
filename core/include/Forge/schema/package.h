#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>

enum class PackageStatus { Resolved, Unresolved };

struct GitSource {
  std::string url{};
  std::string sha256{};
  std::string commit_hash{};
};

struct Package {
  std::string package_name{};
  std::string version{};
  GitSource source{};
};

struct ResolvedPackage {
  std::string name{};
  GitSource source{};
};

#endif // !PACKAGE_H
