#include <stdio.h>
#include <assert.h>

#include <math.h>

#include <stdint.h>

#include "polar.h"

#include "sample_fill.h"

#include "transform_pack.h"

static void apply_polar(polar *p, transform_pack *tp) {

  p->r = sqrt(tp->X[(tp->n+2)%3]);
  p->theta = M_PI * tp->X[(tp->n+1)%3];
  p->phi = 2 * M_PI * tp->X[tp->n];

}

typedef int (*completion_func_t)(transform_pack *tp, void *extra);

static int lag_fill(char **buffer, char *bend, sample_fill *s, transform_pack *tp, completion_func_t completion_func, void *extra) {

  char *buf;

  int j;

  assert(tp != NULL);
  
  buf = *buffer;

  for (j = tp->jcount; j < tp->lag; j++) {

    if (!s->filled_bytes) {

      s->sample = (unsigned char) *buf; s->filled_bytes++;
      buf++;

    }

    if (tp->mode && s->filled_bytes==1) {

      if (buf < bend) {
	s->sample |= ((uint64_t)((unsigned char)*buf) << SHIFTS);
	s->filled_bytes++;
	buf++;
      }
      else {
	break;
      }

    }

  }

  *buffer = buf;

  tp->jcount = (j == tp->lag) ? 0 : j;

  return 0;

}

static int preload_fill(char **buffer, char *bend, sample_fill *s, transform_pack *tp, completion_func_t completion_func, void *extra) {

  int i;

  assert(tp!=NULL);

  for (i = tp->icount; i < 3; i++) {

    lag_fill(buffer, bend, s, tp, completion_func, extra);

  }

  assert(i <= 3);

  if (i == 3) {
    tp->state |= HAVE_PREFILL;
  }

  return 0;

}

static int process_point(char **buffer, char *bend, sample_fill *s, transform_pack *tp, completion_func_t completion_func, void *extra) {

  assert(tp!=NULL && s!=NULL);

  assert(completion_func != NULL);

  tp->X[tp->n] = (s->sample == 0) ? 0 : s->sample / tp->srange;

  s->filled_bytes = 0;
    
  tp->n = (tp->n+1) % 3;

  if (!tp->sr) {

    completion_func(tp, extra);

  }
    
  if (tp->skip) {
    tp->sr = (tp->sr >= tp->skip) ? 0 : (tp->sr + 1);
  }

  return 0;

}

struct polar_workpack {
  polar *p;
  polar *pend;
};

static void show_transform_pack(transform_pack *tp) {

  printf("%s: %f %f %f\n", __FUNCTION__, tp->X[0], tp->X[1], tp->X[2]);

}

static void show_polar(polar *p) {

  printf("%s: %f %f %f\n", __FUNCTION__, p->r, p->theta, p->phi);

}

static int completion_function(transform_pack *tp, void *extra) {

  struct polar_workpack *pwp = (struct polar_workpack *) extra;

  assert(pwp != NULL);

  assert(pwp->p < pwp->pend);

  apply_polar(pwp->p, tp);

  pwp->p++;

  return 0;

}

int transform_data(transform_pack *tp, char *buf, int len, polar *p, polar **pend) {

  sample_fill *s;

  struct polar_workpack pwp = { p, NULL };

  char *bend =  buf + len;

  int j;

  assert(tp!=NULL && tp->lag >= 0);

  assert(buf!=NULL && len>0 && buf < bend);

  assert(p!=NULL && pend!=NULL);

  assert(p < (*pend));

  s = &(tp->samp);

  j = tp->jcount;

  pwp.pend = *pend;

  while ( buf < bend ) {

    if (! (tp->state & HAVE_PREFILL)) {

      preload_fill(&buf, bend, s, tp, completion_function, &pwp);

      continue;

    }

    if ((tp->mode && s->filled_bytes==2) || ((!tp->mode) && s->filled_bytes==1)) {

      process_point(&buf, bend, s, tp, completion_function, &pwp);

    }

    lag_fill(&buf, bend, s, tp, completion_function, &pwp);

  }

  if (pend!=NULL) {

    *pend = pwp.p;

  }

  tp->jcount = j;

  return 0;

}
