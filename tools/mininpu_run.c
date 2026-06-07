#include "mininpu.h"

#include <stdio.h>

int main(void)
{
    struct mininpu_ctx *ctx = NULL;

    if (mininpu_open(&ctx) != 0) {
        fprintf(stderr, "mini NPU runtime initialization failed\n");
        return 1;
    }

    printf("mini NPU runtime %s initialized\n", mininpu_version());
    mininpu_close(ctx);
    return 0;
}
