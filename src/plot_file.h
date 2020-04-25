#ifndef PLOT_FILE_H
#define PLOT_FILE_H

#include "polar.h"

#include "transform_pack.h"

#include "read_status.h"

int plot_file(char *filename, transform_pack *tp, int (*callback_polar)(polar *p), read_statusfunc read_status);

#endif
