#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "domain/Identifiers.hpp"
#include "ports/EventSink.hpp"

namespace workshop::ports {

template <EventSink Inner>
class SortingEventSink {
 public:
  explicit SortingEventSink(Inner& inner) : inner_(inner) {}

  SortingEventSink(const SortingEventSink&) = delete;
  SortingEventSink& operator=(const SortingEventSink&) = delete;
  SortingEventSink(SortingEventSink&&) = delete;
  SortingEventSink& operator=(SortingEventSink&&) = delete;

  void OnStart(domain::TimeType time, domain::ItemId item, domain::OpType type,
               domain::MachineId machine);
  void OnFinish(domain::TimeType time, domain::ItemId item, domain::OpType type,
                domain::MachineId machine);
  void OnWait(domain::TimeType time, domain::ItemId item, domain::OpType type,
              domain::MachineId machine, std::size_t pos);
  void OnReady(domain::TimeType time, domain::ItemId item,
               domain::MachineId machine);
  void OnStop(domain::TimeType time);

 private:
  enum class Kind : uint8_t { Finish = 0, Start = 1, Wait = 2, Ready = 3 };

  struct BufEvent {
    Kind kind;
    domain::TimeType time;
    domain::ItemId item;
    domain::OpType type;
    domain::MachineId machine;
    std::size_t queue_pos;
  };

  void TryFlush(domain::TimeType time);

  void Flush();

  Inner& inner_;
  std::vector<BufEvent> buf_;
};

}  // namespace workshop::ports

#define SORTING_EVENT_SINK_IMPL
#include "ports/SortingEventSink.ipp"
#undef SORTING_EVENT_SINK_IMPL
