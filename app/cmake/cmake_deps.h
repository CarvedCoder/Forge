#pragma once

#include <Forge/schema/manifest.h>

namespace forge::commands {

// Regenerates .forge/deps.cmake from the manifest's package list. Always
// overwrites the whole file — this is the mechanism that lets the
// developer's own CMakeLists.txt never need manual edits after the
// initial `include(.forge/deps.cmake)` line written by `forge init`.
void write_cmake_deps(const ForgeToml& manifest);

} // namespace forge::commands
