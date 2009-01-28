#ifndef PLOT_FILE_H
#define PLOT_FILE_H

#include "polar.h"

#include "transform_pack.h"

#include "read_status.h"

int plot_file(char *filename, struct transform_pack *t, int (*callback_polar)(struct polar *p), read_status_t read_status);

#endif
