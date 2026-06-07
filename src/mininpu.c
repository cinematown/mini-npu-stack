#include "mininpu.h"

#include <errno.h>
#include <stdlib.h>

#define MININPU_VERSION "0.1.0"
#define MININPU_MAX_JOBS 16

struct mininpu_job_entry {
    uint32_t id;
    enum mininpu_job_status status;
    int result;
    int in_use;
};

struct mininpu_ctx {
    struct mininpu_job_entry jobs[MININPU_MAX_JOBS];
    uint32_t next_job_id;
};

typedef int (*mininpu_op_fn)(const int32_t *input_a,
                             const int32_t *input_b,
                             int32_t *output, size_t count);

struct mininpu_op_entry {
    mininpu_op_fn execute;
    int needs_input_b;
};

static int execute_add(const int32_t *input_a, const int32_t *input_b,
                       int32_t *output, size_t count)
{
    size_t i;

    for (i = 0; i < count; i++) {
        output[i] = input_a[i] + input_b[i];
    }

    return 0;
}

static int execute_relu(const int32_t *input_a, const int32_t *input_b,
                        int32_t *output, size_t count)
{
    size_t i;

    (void)input_b;
    for (i = 0; i < count; i++) {
        output[i] = input_a[i] > 0 ? input_a[i] : 0;
    }

    return 0;
}

static const struct mininpu_op_entry ops[MININPU_OP_COUNT] = {
    [MININPU_OP_ADD] = {execute_add, 1},
    [MININPU_OP_RELU] = {execute_relu, 0},
};

static int validate_operation(enum mininpu_op op, const int32_t *input_a,
                              const int32_t *input_b, int32_t *output,
                              size_t count)
{
    const struct mininpu_op_entry *entry;

    if (input_a == NULL || output == NULL || count == 0) {
        return -EINVAL;
    }

    if (op < 0 || op >= MININPU_OP_COUNT) {
        return -EINVAL;
    }

    entry = &ops[op];
    if (entry->execute == NULL || (entry->needs_input_b && input_b == NULL)) {
        return -EINVAL;
    }

    return 0;
}

static struct mininpu_job_entry *find_job(struct mininpu_ctx *ctx,
                                           uint32_t job_id)
{
    size_t i;

    for (i = 0; i < MININPU_MAX_JOBS; i++) {
        if (ctx->jobs[i].in_use && ctx->jobs[i].id == job_id) {
            return &ctx->jobs[i];
        }
    }

    return NULL;
}

static struct mininpu_job_entry *find_free_job(struct mininpu_ctx *ctx)
{
    size_t i;

    for (i = 0; i < MININPU_MAX_JOBS; i++) {
        if (!ctx->jobs[i].in_use) {
            return &ctx->jobs[i];
        }
    }

    return NULL;
}

int mininpu_open(struct mininpu_ctx **out)
{
    struct mininpu_ctx *ctx;

    if (out == NULL) {
        return -EINVAL;
    }

    *out = NULL;
    ctx = calloc(1, sizeof(*ctx));
    if (ctx == NULL) {
        return -ENOMEM;
    }

    ctx->next_job_id = 1;
    *out = ctx;
    return 0;
}

void mininpu_close(struct mininpu_ctx *ctx)
{
    free(ctx);
}

const char *mininpu_version(void)
{
    return MININPU_VERSION;
}

int mininpu_execute(struct mininpu_ctx *ctx, enum mininpu_op op,
                    const int32_t *input_a, const int32_t *input_b,
                    int32_t *output, size_t count)
{
    int result;

    if (ctx == NULL) {
        return -EINVAL;
    }

    result = validate_operation(op, input_a, input_b, output, count);
    if (result != 0) {
        return result;
    }

    return ops[op].execute(input_a, input_b, output, count);
}

int mininpu_submit(struct mininpu_ctx *ctx, const struct mininpu_job *job,
                   uint32_t *job_id)
{
    struct mininpu_job_entry *entry;
    int result;

    if (ctx == NULL || job == NULL || job_id == NULL) {
        return -EINVAL;
    }

    *job_id = 0;
    result = validate_operation(job->op, job->input_a, job->input_b,
                                job->output, job->count);
    if (result != 0) {
        return result;
    }

    entry = find_free_job(ctx);
    if (entry == NULL) {
        return -ENOSPC;
    }

    entry->id = ctx->next_job_id++;
    if (ctx->next_job_id == 0) {
        ctx->next_job_id = 1;
    }
    entry->status = MININPU_JOB_PENDING;
    entry->result = 0;
    entry->in_use = 1;

    entry->result = ops[job->op].execute(job->input_a, job->input_b,
                                         job->output, job->count);
    entry->status = MININPU_JOB_COMPLETED;
    *job_id = entry->id;
    return 0;
}

int mininpu_job_status(struct mininpu_ctx *ctx, uint32_t job_id,
                       enum mininpu_job_status *status)
{
    struct mininpu_job_entry *entry;

    if (ctx == NULL || job_id == 0 || status == NULL) {
        return -EINVAL;
    }

    entry = find_job(ctx, job_id);
    if (entry == NULL) {
        return -ENOENT;
    }

    *status = entry->status;
    return 0;
}

int mininpu_wait(struct mininpu_ctx *ctx, uint32_t job_id)
{
    struct mininpu_job_entry *entry;
    int result;

    if (ctx == NULL || job_id == 0) {
        return -EINVAL;
    }

    entry = find_job(ctx, job_id);
    if (entry == NULL) {
        return -ENOENT;
    }

    if (entry->status != MININPU_JOB_COMPLETED) {
        return -EAGAIN;
    }

    result = entry->result;
    entry->in_use = 0;
    return result;
}
