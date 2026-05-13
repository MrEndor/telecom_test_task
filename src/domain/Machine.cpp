#include "domain/Machine.hpp"

namespace workshop::domain {

void Machine::Enqueue(Item item) {
  wait_time_ += timings_[item.type][id_];
  queue_.push_back(item);
}

Item Machine::Dequeue() {
  Item head = queue_.front();
  wait_time_ -= timings_[head.type][id_];
  queue_.pop_front();
  busy_ = true;
  return head;
}

}  // namespace workshop::domain
