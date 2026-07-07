#pragma once
#ifndef LOCK_H
#define LOCK_H

#include <Forge/schema/abi.h>
#include <Forge/schema/metadata.h>
#include <Forge/schema/package.h>
#include <Forge/schema/project.h>
#include <vector>

namespace forge {

struct ForgeLock {
  Metadata metadata{};
  forge::Abi abi{};
  std::vector<ResolvedPackage> resolved_packages{};
};

struct ForgeToml {
  forge::ProjectInfo project{};
  std::vector<forge::Package> packages{};
  forge::Abi abi{};
  forge::Metadata metadata{};
};

} // namespace forge
#endif // !LOCK_H
