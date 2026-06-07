#include "mininpu.h"

#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const int32_t input_a[] = {-3, 2, -1, 4};
    const int32_t input_b[] = {1, -5, 3, 2};
    int32_t values[4];
    struct mininpu_ctx *ctx = NULL;
    size_t i;

    if (mininpu_open(&ctx) != 0) {
        return 1;
    }

    if (mininpu_execute(ctx, MININPU_OP_ADD, input_a, input_b,
                        values, 4) != 0 ||
        mininpu_execute(ctx, MININPU_OP_RELU, values, NULL,
                        values, 4) != 0) {
        mininpu_close(ctx);
        return 1;
    }

    printf("ADD + RELU:");
    for (i = 0; i < 4; i++) {
        printf(" %d", values[i]);
    }
    printf("\n");

    mininpu_close(ctx);
    return 0;
}
