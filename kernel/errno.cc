#include <kernel/config.h>

int __errnos[MAX_TID] __attribute__((section(".app.data")));
