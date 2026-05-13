#pragma once

#include <istream>
#include <stdexcept>
#include <string>

#include "config/WorkshopConfig.hpp"

namespace workshop::config {

class ParseError : public std::runtime_error {
 public:
  explicit ParseError(std::string offending_line)
      : std::runtime_error(std::move(offending_line)) {}
};

[[nodiscard]] WorkshopConfig ParseWorkshopConfig(std::istream& istream);

}  // namespace workshop::config
