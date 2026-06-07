#include "mininpu.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    struct mininpu_context context = {0};

    assert(!mininpu_is_initialized(&context));
    assert(mininpu_init(&context) == 0);
    assert(mininpu_is_initialized(&context));

    mininpu_shutdown(&context);
    assert(!mininpu_is_initialized(&context));
    assert(mininpu_init(NULL) != 0);

    printf("test_mininpu_basic: passed\n");
    return 0;
}
