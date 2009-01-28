#ifndef SAMPLE_FILL_H
#define SAMPLE_FILL_H

typedef unsigned int SAMPLE;
#define SHIFTS 8

struct sample_fill {
  SAMPLE sample;
  int filled_bytes;
};

#endif
