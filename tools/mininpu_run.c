#include "mininpu.h"

#include <stdio.h>

int main(void)
{
    struct mininpu_context context = {0};

    if (mininpu_init(&context) != 0) {
        fprintf(stderr, "mini NPU runtime initialization failed\n");
        return 1;
    }

    printf("mini NPU runtime initialized\n");
    mininpu_shutdown(&context);
    return 0;
}
