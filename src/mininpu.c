#include "mininpu.h"

#include <stddef.h>

int mininpu_init(struct mininpu_context *context)
{
    if (context == NULL) {
        return -1;
    }

    context->initialized = true;
    return 0;
}

void mininpu_shutdown(struct mininpu_context *context)
{
    if (context != NULL) {
        context->initialized = false;
    }
}

bool mininpu_is_initialized(const struct mininpu_context *context)
{
    return context != NULL && context->initialized;
}
