# Architecture

## Runtime Context

Applications create an opaque `mininpu_ctx` with `mininpu_open` and release it
with `mininpu_close`. The context owns simulated device state, including a
fixed-size table of 16 jobs.

## Operator Dispatch

`mininpu_execute` accepts an operator ID and plain `int32_t` input and output
buffers. The runtime validates the request, indexes a fixed internal ops table,
and calls the selected function pointer.

Each table entry contains:

- the operator implementation function
- whether the operator requires a second input buffer

`ADD` requires two inputs and performs element-wise addition. `RELU` uses one
input and replaces negative values with zero. The table is compiled into the
runtime; there is no plugin loading or tensor framework.

## Job Lifecycle

A `mininpu_job` describes one operator invocation. `mininpu_submit` validates
the descriptor, allocates a slot in the context job table, assigns a nonzero
job ID, and marks the job pending.

Execution is synchronous for now:

1. **Submit:** reserve a job-table slot and assign its ID.
2. **Execute:** dispatch the operator through the ops table immediately.
3. **Complete:** store the result and mark the job `MININPU_JOB_COMPLETED`.
4. **Wait:** return the stored result and release the job-table slot.

`mininpu_job_status` can inspect completion before `mininpu_wait`. There are no
worker threads, interrupts, or kernel interfaces yet; the job model only
establishes the submission and completion API boundary.
