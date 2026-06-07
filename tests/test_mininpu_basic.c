#include "mininpu.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void test_context_and_version(void)
{
    struct mininpu_ctx *ctx = NULL;
    const char *version = mininpu_version();

    assert(version != NULL);
    assert(strlen(version) > 0);
    assert(mininpu_open(NULL) == -EINVAL);
    assert(mininpu_open(&ctx) == 0);
    assert(ctx != NULL);

    mininpu_close(ctx);
    mininpu_close(NULL);
}

static void test_add(void)
{
    const int32_t input_a[] = {1, -2, 3, 0};
    const int32_t input_b[] = {4, 2, -5, 7};
    const int32_t expected[] = {5, 0, -2, 7};
    int32_t output[4] = {0};
    struct mininpu_ctx *ctx = NULL;

    assert(mininpu_open(&ctx) == 0);
    assert(mininpu_execute(ctx, MININPU_OP_ADD, input_a, input_b,
                           output, 4) == 0);
    assert(memcmp(output, expected, sizeof(expected)) == 0);
    mininpu_close(ctx);
}

static void test_relu(void)
{
    const int32_t input[] = {-4, 0, 3, -1, 8};
    const int32_t expected[] = {0, 0, 3, 0, 8};
    int32_t output[5] = {0};
    struct mininpu_ctx *ctx = NULL;

    assert(mininpu_open(&ctx) == 0);
    assert(mininpu_execute(ctx, MININPU_OP_RELU, input, NULL,
                           output, 5) == 0);
    assert(memcmp(output, expected, sizeof(expected)) == 0);
    mininpu_close(ctx);
}

static void test_execute_validation(void)
{
    int32_t value = 1;
    int32_t output = 0;
    struct mininpu_ctx *ctx = NULL;

    assert(mininpu_open(&ctx) == 0);
    assert(mininpu_execute(NULL, MININPU_OP_RELU, &value, NULL,
                           &output, 1) == -EINVAL);
    assert(mininpu_execute(ctx, MININPU_OP_ADD, &value, NULL,
                           &output, 1) == -EINVAL);
    assert(mininpu_execute(ctx, MININPU_OP_RELU, &value, NULL,
                           &output, 0) == -EINVAL);
    assert(mininpu_execute(ctx, MININPU_OP_COUNT, &value, NULL,
                           &output, 1) == -EINVAL);
    mininpu_close(ctx);
}

static void test_submit_and_wait(void)
{
    const int32_t input_a[] = {1, -5, 3};
    const int32_t input_b[] = {2, 1, -8};
    const int32_t expected[] = {3, -4, -5};
    int32_t output[3] = {0};
    struct mininpu_ctx *ctx = NULL;
    struct mininpu_job job = {
        .op = MININPU_OP_ADD,
        .input_a = input_a,
        .input_b = input_b,
        .output = output,
        .count = 3,
    };
    enum mininpu_job_status status = MININPU_JOB_PENDING;
    uint32_t job_id = 0;

    assert(mininpu_open(&ctx) == 0);
    assert(mininpu_submit(ctx, &job, &job_id) == 0);
    assert(job_id != 0);
    assert(memcmp(output, expected, sizeof(expected)) == 0);
    assert(mininpu_job_status(ctx, job_id, &status) == 0);
    assert(status == MININPU_JOB_COMPLETED);
    assert(mininpu_wait(ctx, job_id) == 0);
    assert(mininpu_job_status(ctx, job_id, &status) == -ENOENT);
    assert(mininpu_wait(ctx, job_id) == -ENOENT);
    mininpu_close(ctx);
}

static void test_submit_validation(void)
{
    int32_t input = -1;
    int32_t output = 0;
    struct mininpu_ctx *ctx = NULL;
    struct mininpu_job job = {
        .op = MININPU_OP_RELU,
        .input_a = &input,
        .input_b = NULL,
        .output = &output,
        .count = 1,
    };
    uint32_t job_id = 7;

    assert(mininpu_open(&ctx) == 0);
    assert(mininpu_submit(NULL, &job, &job_id) == -EINVAL);
    assert(mininpu_submit(ctx, NULL, &job_id) == -EINVAL);
    assert(mininpu_submit(ctx, &job, NULL) == -EINVAL);
    assert(mininpu_submit(ctx, &job, &job_id) == 0);
    assert(job_id != 0);
    assert(output == 0);
    assert(mininpu_wait(ctx, job_id) == 0);
    mininpu_close(ctx);
}

int main(void)
{
    test_context_and_version();
    test_add();
    test_relu();
    test_execute_validation();
    test_submit_and_wait();
    test_submit_validation();

    printf("test_mininpu_basic: passed\n");
    return 0;
}
