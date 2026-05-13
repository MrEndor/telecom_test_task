#ifndef STREAM_EVENT_SINK_IMPL
#error "Do not include this file directly"
#endif

namespace workshop::ports {

inline void StreamEventSink::OnStart(domain::TimeType time,
                                     domain::ItemId item_id,
                                     domain::OpType op_type,
                                     domain::MachineId machine_id) {
  std::format_to(std::ostreambuf_iterator<char>(os_), "start {} {} {} {}\n",
                 time, item_id, op_type, machine_id);
}

inline void StreamEventSink::OnFinish(domain::TimeType time,
                                      domain::ItemId item_id,
                                      domain::OpType op_type,
                                      domain::MachineId machine_id) {
  std::format_to(std::ostreambuf_iterator<char>(os_), "finish {} {} {} {}\n",
                 time, item_id, op_type, machine_id);
}

inline void StreamEventSink::OnWait(domain::TimeType time,
                                    domain::ItemId item_id,
                                    domain::OpType op_type,
                                    domain::MachineId machine_id,
                                    size_t queue_position) {
  std::format_to(std::ostreambuf_iterator<char>(os_), "wait {} {} {} {} {}\n",
                 time, item_id, op_type, machine_id, queue_position);
}

inline void StreamEventSink::OnReady(domain::TimeType time,
                                     domain::ItemId item_id,
                                     domain::MachineId machine_id) {
  std::format_to(std::ostreambuf_iterator<char>(os_), "ready {} {} {}\n", time,
                 item_id, machine_id);
}

inline void StreamEventSink::OnStop(domain::TimeType time) {
  std::format_to(std::ostreambuf_iterator<char>(os_), "stop {}\n", time);
}

}  // namespace workshop::ports