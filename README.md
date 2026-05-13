# Workshop Simulator

Discrete-event simulator for a sequential manufacturing workshop.

## Prerequisites

- `cmake >= 3.20`
- `g++` or `clang++` with C++20 support
- `just` (optional)

## Quick start

1. Build

```bash
cmake --preset release
cmake --build --preset release --parallel
```

or

```bash
just build
```

2. Run

```bash
./build/src/workshop_model input.txt
```

or

```bash
just run input.txt
```

On format error, the offending line is printed to stderr and the program exits with code 1.
