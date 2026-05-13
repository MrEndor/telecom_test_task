include_guard(GLOBAL)

add_library(workshop_project_options INTERFACE)

target_compile_features(workshop_project_options
        INTERFACE
        cxx_std_20)

set_target_properties(workshop_project_options
        PROPERTIES
        INTERFACE_CXX_EXTENSIONS OFF)

option(WORKSHOP_BUILD_TESTS       "Build unit and e2e tests"        OFF)
option(WORKSHOP_ENABLE_COVERAGE   "Instrument for line coverage"    OFF)
option(WORKSHOP_ENABLE_SANITIZERS "Enable ASAN + UBSAN on targets"  OFF)
