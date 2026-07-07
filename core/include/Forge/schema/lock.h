#ifndef LOCK_H
#define LOCK_H

#include <Forge/abi.h>
#include <Forge/metadata.h>
#include <Forge/package.h>

#include <vector>

struct ForgeLock;

struct ForgeLock {
  Metadata metadata{};
  Abi abi{};
  std::vector<ResolvedPackage> resolved_packages{};
};

#endif // !LOCK_H
