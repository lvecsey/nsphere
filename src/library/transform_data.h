#ifndef TRANSFORM_DATA_H
#define TRANSFORM_DATA_H

#include "sample_fill.h"

#include "transform_pack.h"

int transform_data(struct transform_pack *t, char *buf, int len, struct polar *p, struct polar **pend);

#endif
