#ifndef SIMULATOR_IMPL
#error "Do not include this file directly"
#endif

namespace workshop::modeling {

template <ports::EventSink Sink>
Simulator<Sink>::Simulator(config::WorkshopConfig cfg, Sink& sink)
    : cfg_(std::move(cfg)), sink_(sink) {
  machines_.reserve(cfg_.num_machines);

  for (domain::MachineId i = 0; i < cfg_.num_machines; ++i) {
    machines_.emplace_back(i, cfg_.timings);
    for (const domain::Item& item : cfg_.initial_queues[i]) {
      machines_.back().Enqueue(item);
    }
  }
}

template <ports::EventSink Sink>
void Simulator<Sink>::Run() {
  SeedMachines();

  domain::TimeType last_time = 0;
  while (!events_.IsEmpty()) {
    last_time = events_.PeekMinTime();
    for (const auto& event : events_.PopBatchAtMinTime()) {
      OnFinish(event);
    }
  }

  sink_.OnStop(last_time);
}

template <ports::EventSink Sink>
void Simulator<Sink>::LaunchOn(domain::Item item, domain::MachineId machine_id,
                               domain::TimeType now) {
  sink_.OnStart(now, item.id, item.type, machine_id);
  ScheduleFinish(item, machine_id, now);
}

template <ports::EventSink Sink>
void Simulator<Sink>::SeedMachines() {
  for (domain::MachineId i = 0; i < cfg_.num_machines; ++i) {
    domain::Machine& machine = machines_[i];
    if (machine.IsQueueEmpty()) {
      continue;
    }

    domain::Item item = machine.Dequeue();
    LaunchOn(item, i, 0);
  }
}

template <ports::EventSink Sink>
void Simulator<Sink>::OnFinish(const domain::FinishEvent& event) {
  machines_[event.machine_id].EndJob();
  sink_.OnFinish(event.time, event.item.id, event.item.type, event.machine_id);
  RouteToNextOp(event);
  StartNextQueued(event.machine_id, event.time);
}

template <ports::EventSink Sink>
void Simulator<Sink>::RouteToNextOp(const domain::FinishEvent& event) {
  domain::OpType next_type = event.item.type + 1;
  if (next_type == cfg_.num_types - 1) {
    sink_.OnReady(event.time, event.item.id, event.machine_id);
    return;
  }

  AssignItem(domain::Item{event.item.id, next_type}, event.time);
}

template <ports::EventSink Sink>
void Simulator<Sink>::AssignItem(domain::Item item, domain::TimeType now) {
  auto it = std::ranges::min_element(machines_, {}, &domain::Machine::WaitTime);
  domain::MachineId target = it->Id();
  domain::Machine& machine = machines_[target];

  if (machine.IsBusy()) {
    size_t prev_size = machine.QueueSize();
    machine.Enqueue(item);
    sink_.OnWait(now, item.id, item.type, target, prev_size);
    return;
  }

  machine.BeginJob();
  LaunchOn(item, target, now);
}

template <ports::EventSink Sink>
void Simulator<Sink>::StartNextQueued(domain::MachineId machine_id,
                                      domain::TimeType now) {
  domain::Machine& machine = machines_[machine_id];
  if (machine.IsBusy() || machine.IsQueueEmpty()) {
    return;
  }

  domain::Item item = machine.Dequeue();
  LaunchOn(item, machine_id, now);
}

template <ports::EventSink Sink>
void Simulator<Sink>::ScheduleFinish(domain::Item item,
                                     domain::MachineId machine_id,
                                     domain::TimeType now) {
  domain::TimeType duration = cfg_.timings[item.type][machine_id];
  events_.Schedule(domain::FinishEvent{now + duration, machine_id, item});
}

}  // namespace workshop::modeling
