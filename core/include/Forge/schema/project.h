#pragma once
#ifndef PROJECT_H
#define PROJECT_H

#include <string>
namespace forge {
struct ProjectInfo {
  std::string project_name{};
  std::string project_standard{};
  std::string project_build_type{};
};

} // namespace forge

#endif // !PROJECT_H
