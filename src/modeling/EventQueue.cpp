#include "modeling/EventQueue.hpp"

#include <span>

#include "domain/Event.hpp"
#include "domain/Identifiers.hpp"

namespace workshop::modeling {

std::span<const domain::FinishEvent> EventQueue::PopBatchAtMinTime() {
  batch_.clear();

  if (heap_.empty()) {
    return batch_;
  }

  domain::TimeType min_time = heap_.top().time;
  while (!heap_.empty() && heap_.top().time == min_time) {
    batch_.push_back(heap_.top());
    heap_.pop();
  }

  return batch_;
}

}  // namespace workshop::modeling
