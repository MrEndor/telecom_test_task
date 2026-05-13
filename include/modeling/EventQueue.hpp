#pragma once

#include <functional>
#include <queue>
#include <span>
#include <vector>

#include "domain/Event.hpp"
#include "domain/Identifiers.hpp"

namespace workshop::modeling {

class EventQueue {
 public:
  void Schedule(domain::FinishEvent event) { heap_.push(event); }

  [[nodiscard]] bool IsEmpty() const noexcept { return heap_.empty(); }

  // Precondition: !IsEmpty()
  // Asymptote: O(1)
  [[nodiscard]] domain::TimeType PeekMinTime() const noexcept {
    return heap_.top().time;
  }

  [[nodiscard]] std::span<const domain::FinishEvent> PopBatchAtMinTime();

 private:
  std::priority_queue<domain::FinishEvent, std::vector<domain::FinishEvent>,
                      std::greater<>>
      heap_;
  std::vector<domain::FinishEvent> batch_;
};

}  // namespace workshop::modeling
