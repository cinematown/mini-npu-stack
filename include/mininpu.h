#ifndef MININPU_H
#define MININPU_H

#include <stdbool.h>

struct mininpu_context {
    bool initialized;
};

int mininpu_init(struct mininpu_context *context);
void mininpu_shutdown(struct mininpu_context *context);
bool mininpu_is_initialized(const struct mininpu_context *context);

#endif
