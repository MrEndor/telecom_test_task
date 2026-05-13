#pragma once

#include <vector>

#include "domain/Identifiers.hpp"
#include "domain/Item.hpp"

namespace workshop::config {

struct WorkshopConfig {
  uint32_t num_types{0};
  uint32_t num_machines{0};
  domain::TimingMatrix timings;
  std::vector<std::vector<domain::Item>> initial_queues;
};

}  // namespace workshop::config
