#ifndef TRANSFORM_DATA_H
#define TRANSFORM_DATA_H

#include "sample_fill.h"

#include "transform_pack.h"

#include "polar.h"

int transform_data(transform_pack *tp, char *buf, int len, polar *p, polar **pend);

#endif
