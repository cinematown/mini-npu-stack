# Roadmap

## Phase 1: Runtime Skeleton

- Define a minimal public C API.
- Build a command-line runtime client.
- Add a dependency-free test target.

## Phase 2: Device Simulation

- Represent a simulated NPU device and its state.
- Add host-visible buffers with explicit ownership.
- Model a small command queue.

## Phase 3: Command Execution

- Define a compact command format.
- Execute deterministic placeholder operations.
- Report invalid commands and device state errors.

## Phase 4: Driver Boundary

- Specify a character-device API using open, close, ioctl, and mmap concepts.
- Keep the user-space backend available for tests.
- Document synchronization and memory-lifetime rules.

## Phase 5: Optional Linux Driver

- Implement a pseudo accelerator character driver.
- Connect the runtime to the driver interface.
- Add integration tests where the host environment permits.

Real model loading and neural network inference are outside the current scope.
