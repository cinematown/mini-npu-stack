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
./examples/add_relu_demo
make test
```

Use `make clean` to remove generated files.

## Public API

Include `mininpu.h` to use the runtime:

```c
struct mininpu_ctx *ctx = NULL;
int32_t a[] = {1, -2};
int32_t b[] = {3, 4};
int32_t output[2];

if (mininpu_open(&ctx) == 0) {
    mininpu_execute(ctx, MININPU_OP_ADD, a, b, output, 2);
    mininpu_close(ctx);
}
```

- `mininpu_open` creates an opaque runtime context. It returns `0` on success
  or a negative error code for invalid arguments or allocation failure.
- `mininpu_close` releases a context. Passing `NULL` is allowed.
- `mininpu_version` returns the runtime version as a static string.
- `mininpu_execute` runs `ADD` or `RELU` over plain `int32_t` buffers.

The context internals and operator table are private to the runtime
implementation.

## Layout

- `include/`: public runtime API
- `src/`: runtime implementation
- `tools/`: command-line programs
- `examples/`: small runtime examples
- `tests/`: runtime tests
- `docs/`: design notes and roadmap

## Milestones

1. Establish a C11 user-space runtime skeleton and test harness.
2. Model a small simulated device, command queue, and memory buffers.
3. Add deterministic command execution and error handling.
4. Define a user/kernel interface suitable for a character driver.
5. Optionally implement a Linux pseudo accelerator driver.

See [docs/roadmap.md](docs/roadmap.md) for scope and progression.
