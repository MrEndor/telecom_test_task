#ifndef SORTING_EVENT_SINK_IMPL
#error "Do not include this file directly"
#endif

namespace workshop::ports {
template <EventSink Inner>
void SortingEventSink<Inner>::OnStart(domain::TimeType time,
                                      domain::ItemId item, domain::OpType type,
                                      domain::MachineId machine) {
  TryFlush(time);
  buf_.emplace_back(Kind::Start, time, item, type, machine, 0);
}

template <EventSink Inner>
void SortingEventSink<Inner>::OnFinish(domain::TimeType time,
                                       domain::ItemId item, domain::OpType type,
                                       domain::MachineId machine) {
  TryFlush(time);
  buf_.emplace_back(Kind::Finish, time, item, type, machine, 0);
}

template <EventSink Inner>
void SortingEventSink<Inner>::OnWait(domain::TimeType time, domain::ItemId item,
                                     domain::OpType type,
                                     domain::MachineId machine,
                                     std::size_t pos) {
  TryFlush(time);
  buf_.emplace_back(Kind::Wait, time, item, type, machine, pos);
}

template <EventSink Inner>
void SortingEventSink<Inner>::OnReady(domain::TimeType time,
                                      domain::ItemId item,
                                      domain::MachineId machine) {
  TryFlush(time);
  buf_.emplace_back(Kind::Ready, time, item, 0, machine, 0);
}

template <EventSink Inner>
void SortingEventSink<Inner>::OnStop(domain::TimeType time) {
  Flush();
  inner_.OnStop(time);
}

template <EventSink Inner>
void SortingEventSink<Inner>::TryFlush(domain::TimeType time) {
  if (!buf_.empty() && buf_.front().time != time) {
    Flush();
  }
}

template <EventSink Inner>
void SortingEventSink<Inner>::Flush() {
  std::stable_sort(buf_.begin(), buf_.end(),
                   [](const BufEvent& lhs, const BufEvent& rhs) {
                     if (lhs.kind != rhs.kind) {
                       return lhs.kind < rhs.kind;
                     }
                     return lhs.machine < rhs.machine;
                   });
  for (const auto& evt : buf_) {
    switch (evt.kind) {
      case Kind::Finish:
        inner_.OnFinish(evt.time, evt.item, evt.type, evt.machine);
        break;
      case Kind::Start:
        inner_.OnStart(evt.time, evt.item, evt.type, evt.machine);
        break;
      case Kind::Wait:
        inner_.OnWait(evt.time, evt.item, evt.type, evt.machine, evt.queue_pos);
        break;
      case Kind::Ready:
        inner_.OnReady(evt.time, evt.item, evt.machine);
        break;
      default:;
    }
  }
  buf_.clear();
}

}  // namespace workshop::ports