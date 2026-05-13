#pragma once

#include <cstdint>
#include <vector>

namespace workshop::domain {

using ItemId = std::uint32_t;     // [0, S-1]
using MachineId = std::uint32_t;  // [0, N-1]
using OpType = std::uint32_t;     // [0, M-2]

using TimeType = std::int64_t;
using TimingMatrix = std::vector<std::vector<TimeType>>;

}  // namespace workshop::domain
