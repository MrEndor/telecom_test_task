#pragma once

#include <compare>

#include "domain/Item.hpp"

namespace workshop::domain {

struct FinishEvent {
  TimeType time;
  MachineId machine_id;
  Item item;
};

[[nodiscard]] inline auto operator<=>(const FinishEvent& lhs,
                                      const FinishEvent& rhs) noexcept {
  if (auto cmp = lhs.time <=> rhs.time; cmp != 0) {
    return cmp;
  }
  return lhs.machine_id <=> rhs.machine_id;
}

}  // namespace workshop::domain
