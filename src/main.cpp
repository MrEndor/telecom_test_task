#include <fstream>
#include <iostream>
#include <string_view>

#include "config/WorkshopConfig.hpp"
#include "config/WorkshopConfigParser.hpp"
#include "modeling/Simulator.hpp"
#include "ports/SortingEventSink.hpp"
#include "ports/StreamEventSink.hpp"

namespace {
constexpr int kExpectedArgc = 2;

void EnableFastIO() {
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  std::cerr.tie(nullptr);
  std::cin.tie(nullptr);
}
}  // namespace

int main(int argc, char** argv) try {
  using namespace workshop;

  EnableFastIO();

  if (argc != kExpectedArgc) {
    std::cerr << "usage: " << (argc > 0 ? argv[0] : "workshop_sim")
              << " <input-file>\n";
    return 1;
  }

  std::ifstream istream(argv[1]);
  if (!istream) {
    std::cerr << "Cannot open: " << std::string_view(argv[1]) << '\n';
    return 1;
  }

  auto cfg = config::ParseWorkshopConfig(istream);

  ports::StreamEventSink stream_sink(std::cout);
  ports::SortingEventSink sink(stream_sink);
  modeling::Simulator(cfg, sink).Run();
} catch (const workshop::config::ParseError& e) {
  std::cerr << e.what() << '\n';
  return 1;
}
