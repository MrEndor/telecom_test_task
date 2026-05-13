#pragma once

#include <concepts>

#include "domain/Identifiers.hpp"

namespace workshop::ports {

template <typename T>
concept EventSink =
    requires(T& sink, domain::TimeType time, domain::ItemId item,
             domain::OpType optype, domain::MachineId mid, std::size_t pos) {
      { sink.OnStart(time, item, optype, mid) } -> std::same_as<void>;
      { sink.OnFinish(time, item, optype, mid) } -> std::same_as<void>;
      { sink.OnWait(time, item, optype, mid, pos) } -> std::same_as<void>;
      { sink.OnReady(time, item, mid) } -> std::same_as<void>;
      { sink.OnStop(time) } -> std::same_as<void>;
    };

}  // namespace workshop::ports
