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

#ifndef CAPFRAME_H
#define CAPFRAME_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdint.h>

#include <GL/gl.h>

enum { CF_UNINITIALIZED, CF_ONESHOT, CF_COMPLETED };

typedef struct {

  int fd;
  struct stat buf;
  void *m;

  GLsizei width;
  GLsizei height;

  char *out_fn;

  uint64_t state;
  
} capframe;

capframe init_capframe(long int width, long int height, char *base_fn);

int process_capframe(capframe *cf);

int cleanup_capframe(capframe *cf);

#endif


