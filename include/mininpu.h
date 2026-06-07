#ifndef MININPU_H
#define MININPU_H

#include <stddef.h>
#include <stdint.h>

struct mininpu_ctx;

enum mininpu_op {
    MININPU_OP_ADD = 0,
    MININPU_OP_RELU,
    MININPU_OP_COUNT
};

enum mininpu_job_status {
    MININPU_JOB_PENDING = 0,
    MININPU_JOB_COMPLETED
};

struct mininpu_job {
    enum mininpu_op op;
    const int32_t *input_a;
    const int32_t *input_b;
    int32_t *output;
    size_t count;
};

int mininpu_open(struct mininpu_ctx **out);
void mininpu_close(struct mininpu_ctx *ctx);
const char *mininpu_version(void);
int mininpu_execute(struct mininpu_ctx *ctx, enum mininpu_op op,
                    const int32_t *input_a, const int32_t *input_b,
                    int32_t *output, size_t count);
int mininpu_submit(struct mininpu_ctx *ctx, const struct mininpu_job *job,
                   uint32_t *job_id);
int mininpu_job_status(struct mininpu_ctx *ctx, uint32_t job_id,
                       enum mininpu_job_status *status);
int mininpu_wait(struct mininpu_ctx *ctx, uint32_t job_id);

#endif
