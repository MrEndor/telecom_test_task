#pragma once

#include <format>
#include <iterator>
#include <ostream>

#include "domain/Identifiers.hpp"

namespace workshop::ports {

class StreamEventSink final {
 public:
  explicit StreamEventSink(std::ostream& os) : os_(os) {}

  void OnStart(domain::TimeType time, domain::ItemId item_id,
               domain::OpType op_type, domain::MachineId machine_id);
  void OnFinish(domain::TimeType time, domain::ItemId item_id,
                domain::OpType op_type, domain::MachineId machine_id);
  void OnWait(domain::TimeType time, domain::ItemId item_id,
              domain::OpType op_type, domain::MachineId machine_id,
              size_t queue_position);
  void OnReady(domain::TimeType time, domain::ItemId item_id,
               domain::MachineId machine_id);
  void OnStop(domain::TimeType time);

 private:
  std::ostream& os_;
};

}  // namespace workshop::ports

#define STREAM_EVENT_SINK_IMPL
#include "ports/StreamEventSink.ipp"
#undef STREAM_EVENT_SINK_IMPL
