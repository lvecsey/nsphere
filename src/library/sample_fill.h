#ifndef SAMPLE_FILL_H
#define SAMPLE_FILL_H

#include <stdint.h>

#define SHIFTS 8

typedef struct {

  uint64_t sample;
  int filled_bytes;

} sample_fill;

#endif
