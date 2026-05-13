cmake_minimum_required(VERSION 3.20)

add_library(workshop_core STATIC
        domain/Machine.cpp
        config/WorkshopConfigParser.cpp
        modeling/EventQueue.cpp
)

target_include_directories(workshop_core
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(workshop_core
        PUBLIC
        workshop_project_options
        workshop_compiler_warnings
)

workshop_enable_sanitizers(workshop_core)
workshop_enable_coverage(workshop_core)

add_executable(workshop_model main.cpp)

target_link_libraries(workshop_model
        PRIVATE
        workshop_core
)

workshop_enable_sanitizers(workshop_model)
workshop_enable_coverage(workshop_model)
