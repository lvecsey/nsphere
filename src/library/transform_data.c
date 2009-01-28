#include <stdio.h>
#include <assert.h>

#include <math.h>

#include "polar.h"

#include "sample_fill.h"

#include "transform_pack.h"

static void apply_polar(struct polar *p, struct transform_pack *t) {

  assert(p!=NULL && t!=NULL);

  p->r = sqrt(t->X[2]);
  p->theta = M_PI * t->X[1];
  p->phi = 2 * M_PI * t->X[0];

}

typedef int (*completion_func_t)(struct transform_pack *t, void *extra);

static int attempt_fill(char **buffer, char *bend, struct sample_fill *s, struct transform_pack *t, completion_func_t completion_func, void *extra) {

  char *buf;

  assert(buffer!=NULL);

  assert(completion_func!=NULL);

  buf = *buffer;

  if (!s->filled_bytes) {

    s->sample = (unsigned char) *buf; s->filled_bytes++;
    buf++;

  }

  if (t->mode && s->filled_bytes==1) {

    if (buf < bend) {
      s->sample |= ((SAMPLE)((unsigned char)*buf) << SHIFTS);
      s->filled_bytes++;
      buf++;
    }
    else {
      *buffer = buf;
      return 0;
    }
  }

  assert_transform_pack(t);

  if ((t->mode && s->filled_bytes==2) || ((!t->mode) && s->filled_bytes==1)) {

    *t->x = s->sample / t->srange;

    t->x = (t->x + 1 >= t->xend) ? t->X : t->x + 1;

    s->filled_bytes = 0;
    
    if (t->x == t->X && !t->sr) {

      completion_func(t, extra);

      t->x = t->X;

    }
    
    if (t->skip) {
      t->sr = (t->sr >= t->skip) ? 0 : (t->sr + 1);
    }

  }

  *buffer = buf;
  
  return 0;

}

struct polar_workpack {
  struct polar *p;
  struct polar *pend;
};

static void show_transform_pack(struct transform_pack *t) {

  assert(t!=NULL);

  printf("%s: %f %f %f\n", __FUNCTION__, t->X[0], t->X[1], t->X[2]);

}

static void show_polar(struct polar *p) {

  assert(p!=NULL);

  printf("%s: %f %f %f\n", __FUNCTION__, p->r, p->theta, p->phi);

}

static int completion_function(struct transform_pack *t, void *extra) {

  struct polar_workpack *pwp = (struct polar_workpack *) extra;

  assert(pwp != NULL);

  assert(pwp->p < pwp->pend);

  apply_polar(pwp->p, t);

  pwp->p++;

  return 0;

}

int transform_data(struct transform_pack *t, char *buf, int len, struct polar *p, struct polar **pend) {

  struct sample_fill *s;

  struct polar_workpack pwp = { p, NULL };

  char *bend =  buf + len;

  int j;

  assert(t!=NULL && t->lag >= 0);

  assert(buf!=NULL && len>0 && buf < bend);

  assert(p!=NULL && pend!=NULL);

  assert(p < (*pend));

  s = &t->samp;

  j = t->jcount;

  pwp.pend = *pend;

  for ( ; buf < bend; buf++) {

    attempt_fill(&buf, bend, s, t, completion_function, &pwp);

    for ( ; j < t->lag; j++) {
      buf++;
    }

  }

  if (pend!=NULL) {

    *pend = pwp.p;

  }

  t->jcount = j;

  return 0;

}
