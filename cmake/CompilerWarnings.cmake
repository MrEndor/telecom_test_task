include_guard(GLOBAL)

add_library(workshop_compiler_warnings INTERFACE)

set(workshop_gcc_clang_warnings
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Werror)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(workshop_compiler_warnings
            INTERFACE
            ${workshop_gcc_clang_warnings})
endif ()
