#include "config/WorkshopConfigParser.hpp"

#include <sstream>
#include <string>
#include <vector>

#include "config/WorkshopConfig.hpp"
#include "domain/Identifiers.hpp"

namespace workshop::config {

namespace {

constexpr int32_t kMaxTypes = 100;
constexpr int32_t kMaxMachines = 100;
constexpr int32_t kMaxTiming = 10000;

bool IsEof(std::istringstream& stream) { return (stream >> std::ws).eof(); }

class Parser {
 public:
  explicit Parser(std::istream& istream) : istream_(istream) {}

  WorkshopConfig Parse() {
    ParseHeader();
    ParseTimingMatrix();
    ParseQueues();
    return std::move(accum_);
  }

 private:
  void ParseHeader() {
    if (!ReadNextLine()) {
      throw ParseError(current_line_);
    }

    std::istringstream stream(current_line_);
    int32_t num_types;
    int32_t num_machines;
    if (!(stream >> num_types >> num_machines) || !IsEof(stream)) {
      throw ParseError(current_line_);
    }

    if (num_types < 1 || num_types > kMaxTypes || num_machines < 1 ||
        num_machines > kMaxMachines) {
      throw ParseError(current_line_);
    }

    accum_.num_types = static_cast<uint32_t>(num_types);
    accum_.num_machines = static_cast<uint32_t>(num_machines);
  }

  void ParseTimingMatrix() {
    uint32_t rows = accum_.num_types - 1;
    accum_.timings.assign(rows, {});

    for (uint32_t row = 0; row < rows; ++row) {
      if (!ReadNextLine()) {
        throw ParseError(current_line_);
      }

      std::istringstream stream(current_line_);
      auto& timing_row = accum_.timings[row];
      timing_row.resize(accum_.num_machines);

      for (auto& cell : timing_row) {
        int32_t val;
        if (!(stream >> val) || val < 0 || val > kMaxTiming) {
          throw ParseError(current_line_);
        }
        cell = static_cast<domain::TimeType>(val);
      }

      if (!IsEof(stream)) {
        throw ParseError(current_line_);
      }
    }
  }

  void ParseQueues() {
    accum_.initial_queues.assign(accum_.num_machines, {});
    domain::ItemId next_id = 0;
    for (domain::MachineId idx = 0; idx < accum_.num_machines; ++idx) {
      ParseQueueRow(idx, next_id);
    }
  }

  void ParseQueueRow(domain::MachineId machine_id, domain::ItemId& next_id) {
    if (!ReadNextLine()) {
      throw ParseError(current_line_);
    }

    std::istringstream stream(current_line_);
    int32_t count;
    if (!(stream >> count) || count < 0) {
      throw ParseError(current_line_);
    }

    int32_t max_type = static_cast<int32_t>(accum_.num_types) - 2;
    auto& queue = accum_.initial_queues[machine_id];
    queue.reserve(static_cast<size_t>(count));

    for (int32_t i = 0; i < count; ++i) {
      int32_t type_val{};
      if (!(stream >> type_val) || type_val < 0 || type_val > max_type) {
        throw ParseError(current_line_);
      }
      queue.emplace_back(next_id++, static_cast<domain::OpType>(type_val));
    }

    if (!IsEof(stream)) {
      throw ParseError(current_line_);
    }
  }

  bool ReadNextLine() {
    if (!std::getline(istream_, current_line_)) {
      current_line_.clear();
      return false;
    }
    return true;
  }

  std::istream& istream_;
  std::string current_line_;
  WorkshopConfig accum_;
};

}  // namespace

WorkshopConfig ParseWorkshopConfig(std::istream& istream) {
  return Parser{istream}.Parse();
}

}  // namespace workshop::config
