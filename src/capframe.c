/*
    Capture an OpenGL buffer with glReadPixels to a .rgb output file
    Copyright (C) 2020  Lester Vecsey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <errno.h>

#include <string.h>

#include <GL/gl.h>

#include "capframe.h"

capframe init_capframe(long int width, long int height, char *base_fn) {

  capframe cf;
  
  char fn_buf[240];
  
  int retval;

  size_t len;
  
  retval = sprintf(fn_buf, "%s-%ldx%ld.rgb", base_fn, width, height);

  len = retval;
  
  cf.width = width;
  cf.height = height;

  cf.out_fn = malloc(len + 1);
  if (cf.out_fn == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  memcpy(cf.out_fn, fn_buf, len);
  cf.out_fn[len] = 0;

  cf.state = CF_ONESHOT;
  
  return cf;
  
}

int process_capframe(capframe *cf) {

  int retval;

  size_t fsize;

  mode_t mode;

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  
  cf->fd = open(cf->out_fn, O_RDWR | O_CREAT, mode);
  if (cf->fd == -1) {
    perror("open");
    return -1;
  }

  fsize = cf->width * cf->height * sizeof(uint16_t) * 3;

  retval = ftruncate(cf->fd, fsize);
  if (retval == -1) {
    perror("ftruncate");
    return -1;
  }
  
  retval = fstat(cf->fd, & (cf->buf));
  if (retval == -1) {
    perror("fstat");
    return -1;
  }

  cf->m = mmap(NULL, cf->buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, cf->fd, 0);
  if (cf->m == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  glReadPixels(0, 0, cf->width, cf->height, GL_RGB, GL_UNSIGNED_SHORT, cf->m);

  cf->state = CF_COMPLETED;
  
  retval = munmap(cf->m, cf->buf.st_size);
  if (retval == -1) {
    perror("munmap");
    return -1;
  }

  retval = close(cf->fd);
  if (retval == -1) {
    perror("close");
    return -1;
  }
  
  return 0;
  
}

int cleanup_capframe(capframe *cf) {

  free(cf->out_fn);
  
  return 0;

}
