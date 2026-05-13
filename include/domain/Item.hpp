#pragma once

#include "domain/Identifiers.hpp"

namespace workshop::domain {

struct Item {
  ItemId id;
  OpType type;

  [[nodiscard]] bool operator==(const Item&) const noexcept = default;
};

}  // namespace workshop::domain
