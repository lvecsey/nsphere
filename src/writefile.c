/*
    write data to a file descriptor until length specified
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
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int writefile(int fd, void *buf, size_t len) {

  size_t remaining;
  unsigned char *adv_p;
  ssize_t bytes_written;

  size_t chunk_sz = 4096;

  size_t amt;
  
  adv_p = buf;
  
  remaining = len;
  while (remaining > 0) {

    amt = chunk_sz;
    if (remaining < amt) {
      amt = remaining;
    }

    bytes_written = write(fd, adv_p + len - remaining, amt);
    if (!bytes_written) break;
    if (bytes_written < 0) {
      return -1;
    }
    remaining -= amt;

    
  }

  return len;
  
}
