#pragma once

#include <algorithm>
#include <vector>

#include "config/WorkshopConfig.hpp"
#include "domain/Event.hpp"
#include "domain/Identifiers.hpp"
#include "domain/Item.hpp"
#include "domain/Machine.hpp"
#include "modeling/EventQueue.hpp"
#include "ports/EventSink.hpp"

namespace workshop::modeling {

template <ports::EventSink Sink>
class Simulator {
 public:
  Simulator(config::WorkshopConfig cfg, Sink& sink);

  Simulator(const Simulator&) = delete;
  Simulator& operator=(const Simulator&) = delete;
  Simulator(Simulator&&) = delete;
  Simulator& operator=(Simulator&&) = delete;

  void Run();

 private:
  void LaunchOn(domain::Item item, domain::MachineId machine_id,
                domain::TimeType now);

  void SeedMachines();

  void OnFinish(const domain::FinishEvent& event);

  void RouteToNextOp(const domain::FinishEvent& event);

  void AssignItem(domain::Item item, domain::TimeType now);

  void StartNextQueued(domain::MachineId machine_id, domain::TimeType now);

  void ScheduleFinish(domain::Item item, domain::MachineId machine_id,
                      domain::TimeType now);

  config::WorkshopConfig cfg_;
  Sink& sink_;
  std::vector<domain::Machine> machines_;
  EventQueue events_;
};

}  // namespace workshop::modeling

#define SIMULATOR_IMPL
#include "modeling/Simulator.ipp"
#undef SIMULATOR_IMPL
