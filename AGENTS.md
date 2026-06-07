# Project Guidelines

## Scope

Keep this project focused on accelerator runtime and driver concepts. Do not
add real neural network inference or external dependencies.

## Build Rules

- Use C11.
- Compile with `gcc -Wall -Wextra -Werror -g`.
- Keep the public API in `include/mininpu.h`.
- Add or update tests for observable runtime behavior.
- Run `make` and `make test` before finalizing changes.

## Design Rules

- Prefer small C modules and explicit data structures.
- Keep user-space simulation separate from any future kernel code.
- Avoid abstractions until a concrete simulator feature needs them.
