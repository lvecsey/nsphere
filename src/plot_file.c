
#include <stdio.h>
#include <assert.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "polar.h"

#include "transform_pack.h"

#include "transform_data.h"

#include "read_status.h"

int plot_file(char *filename, transform_pack *tp, int (*callback_polar)(polar *p), read_statusfunc read_status) {

  int fd, transform_retval = 0, read_retval = -1;

  char buf[4096];

  unsigned long file_position = 0;

  polar p[4096];

  polar *pend;
  
  //  InitPoints();

  assert(callback_polar != NULL);

  fd = open(filename, O_RDONLY);
  if (fd==-1) {
    fprintf(stderr, "%s: Error opening %s\n", __FUNCTION__, filename);
    return -1;
  }

  tp->srange = tp->mode ? 65536.0 : 256.0;

  do {

    read_retval = read(fd, buf, sizeof(buf));

    if (read_status!=NULL) {
      read_status(file_position, read_retval);
    }

    if (read_retval==-1) {
      fprintf(stderr, "%s: Trouble reading from fd=%d\n", __FUNCTION__, fd);
      return -1;
    }

    file_position += read_retval;

    if (read_retval>0) {

      pend = p + sizeof(p) / sizeof(polar);

      transform_retval = transform_data(tp, buf, read_retval, p, &pend);
      if (transform_retval == -1) {
	fprintf(stderr, "%s: Trouble with transform_data.\n", __FUNCTION__);
	return -1;
      }

      if (callback_polar != NULL) {

	polar *x = p;

	assert (x <= pend);

	for ( ; x < pend; x++) {

	  callback_polar(x);

	}

      }

    }

  } while (read_retval > 0 && transform_retval != -1);

  return 0;

}
