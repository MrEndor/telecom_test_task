#pragma once

#include <deque>

#include "Item.hpp"
#include "domain/Identifiers.hpp"

namespace workshop::domain {

class Machine {
 public:
  Machine(MachineId id, const TimingMatrix& timings)
      : id_(id), timings_(timings) {}

  void Enqueue(Item item);
  Item Dequeue();

  void BeginJob() noexcept { busy_ = true; }
  void EndJob() noexcept { busy_ = false; }

  [[nodiscard]] bool IsQueueEmpty() const noexcept { return queue_.empty(); }
  [[nodiscard]] size_t QueueSize() const noexcept { return queue_.size(); }
  [[nodiscard]] TimeType WaitTime() const noexcept { return wait_time_; }
  [[nodiscard]] MachineId Id() const noexcept { return id_; }
  [[nodiscard]] bool IsBusy() const noexcept { return busy_; }

 private:
  MachineId id_;
  const TimingMatrix& timings_;
  std::deque<Item> queue_;
  TimeType wait_time_{0};
  bool busy_{false};
};

}  // namespace workshop::domain
