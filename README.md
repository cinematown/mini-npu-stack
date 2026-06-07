# mini-npu-stack

`mini-npu-stack` is a small C project for learning AI accelerator system
software. It begins as a user-space runtime simulator with a minimal API and
command-line tool. It does not perform neural network inference.

The project is intended to grow in small, testable stages while keeping the
boundary between applications, the runtime, and a future device interface
clear.

## Build

Requirements:

- GCC
- Make

```sh
make
./tools/mininpu_run
make test
```

Use `make clean` to remove generated files.

## Layout

- `include/`: public runtime API
- `src/`: runtime implementation
- `tools/`: command-line programs
- `tests/`: runtime tests
- `docs/`: design notes and roadmap

## Milestones

1. Establish a C11 user-space runtime skeleton and test harness.
2. Model a small simulated device, command queue, and memory buffers.
3. Add deterministic command execution and error handling.
4. Define a user/kernel interface suitable for a character driver.
5. Optionally implement a Linux pseudo accelerator driver.

See [docs/roadmap.md](docs/roadmap.md) for scope and progression.
