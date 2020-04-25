/*

    Display 3D projection of noise data from file or stream.
    Copyright (C) 2009  Lester Vecsey

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

    Email lvecsey-nsphere@phrasep.com or snail mail to:
    Lester Vecsey, 140 West 69th St, Apt 126c, New York, NY 10023

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include <math.h>
#include <errno.h>
#include <string.h>

#include "polar.h"
#include "cartesian.h"

#include "coordinate_conversion.h"

#include "nsphere.h"

#include "collection_pack.h"

#include "gen_vertexarray.h"

#include "plot_file.h"

#include "transform_pack.h"

#include "prepare_vertices.h"

#include "csv_prepare.h"

#include "verbose_read.h"

#include "capframe.h"

#include "ga.h"

#include "writefile.h"

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLfloat angle = 0.0;

static GLint nsphere1;

uint64_t max_frames;

uint64_t count = 0;

capframe cf;

typedef struct {
  uint16_t r,g,b;
} pixel_t;

int draw_image(pixel_t *rgb, long int xres, long int yres, GLfloat *vertices, GLfloat *colors, long int filled_vertices, double angle) {

  long int vertexno;

  long int xpos, ypos;

  double x, y;

  double aspect;

  GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat blue[4] = {0.0, 0.0, 1.0, 1.0};

  pixel_t fill_color;

  double depth;

  rot3 rot;

  double matrix[16];

  vec3d vFrom, vTo;

  double rot_pnt[3];
  
  vFrom[0] = cos(angle);
  vFrom[1] = 0.0;
  vFrom[2] = sin(angle);
  norm3d(&vFrom);

  vTo[0] = 1.0;
  vTo[1] = 0.0;
  vTo[2] = 0.0;
  norm3d(&vTo);

  rot = create_rotor(vFrom, vTo);
  
  toMatrix3(&rot, matrix);
  
  depth = 1.75;
  
  aspect = ((double) xres) / yres;
  
  for (vertexno = 0; vertexno < filled_vertices; vertexno++) {

    rot_pnt[0] = (vertices[0]) * matrix[0] + (vertices[1]) * matrix[1] + (vertices[2]) * matrix[2] + 1.0 * matrix[3];
    rot_pnt[1] = (vertices[0]) * matrix[4] + (vertices[1]) * matrix[5] + (vertices[2]) * matrix[6] + 1.0 * matrix[7]; 
    rot_pnt[2] = (vertices[0]) * matrix[8] + (vertices[1]) * matrix[9] + (vertices[2]) * matrix[10] + 1.0 * matrix[11];  

    x = rot_pnt[0] / (rot_pnt[2] + depth);
    y = rot_pnt[1] / (rot_pnt[2] + depth);    

    x /= aspect;
    y *= -1.0;

    xpos = x * (xres >> 1); xpos += xres >> 1;
    ypos = y * (yres >> 1); ypos += yres >> 1;    
    
    fill_color.r = colors[0] * 65535;
    fill_color.g = colors[1] * 65535;
    fill_color.b = colors[2] * 65535;    

    rgb[ypos * xres + xpos] = fill_color;
    
    vertices += 3;
    colors += 4;
    
  }

  return 0;

}

int init(GLfloat *vertices, GLfloat *colors, GLfloat *normals, int filled_vertices)
{
  static GLfloat pos1[4] = {2.5, 2.5, -5.0, 0.0};
  static GLfloat pos2[4] = {-2.5, 2.5, -3.0, 0.0};
  static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};

  static GLfloat light_color0[4] = { 1.0, 1.0, 1.0, 1.0 };
  static GLfloat light_color1[4] = { 0.0, 0.0, 1.0, 1.0 };

  glLightfv(GL_LIGHT0, GL_POSITION, pos1);
  glLightfv(GL_LIGHT1, GL_POSITION, pos2);

  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color1);

  glEnable(GL_DEPTH_TEST);

  nsphere1 = glGenLists(1);
  glNewList(nsphere1, GL_COMPILE);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glNormalPointer(GL_FLOAT, 0, normals);

  glDrawArrays(GL_POINTS, 3, filled_vertices);  

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glEndList();

  return 0;
  
}

int prepare_colors(GLfloat *vertices, long int filled_vertices, GLfloat **colors) {

  GLfloat *cp, *end;

  assert(vertices!=NULL && filled_vertices > 0);

  assert(colors!=NULL);

  cp = malloc(4 * sizeof(GLfloat) * filled_vertices);
  if (cp == NULL) {
    fprintf(stderr, "%s: Trouble with malloc for color array.\n", __FUNCTION__);
    return -1;
  }

  *colors = cp;
  
  for (end = cp + 4 * filled_vertices; cp < end; ) {

    *cp++ = *vertices++;
    *cp++ = *vertices++;
    *cp++ = *vertices++;
    *cp++ = 1.0;

  }

  return 0;

}

int prepare_normals(GLfloat *vertices, long int filled_vertices, GLfloat **normals) {

  GLfloat *np, *end;

  double sf;
  
  assert(vertices!=NULL && filled_vertices > 0);

  assert(normals!=NULL);

  np = malloc(3 * sizeof(GLfloat) * filled_vertices);
  if (np == NULL) {
    fprintf(stderr, "%s: Trouble with malloc for normal array.\n", __FUNCTION__);
    return -1;
  }

  *normals = np;
  
  for (end = np + 3 * filled_vertices; np < end; ) {
    
    sf = sqrt(vertices[0] * vertices[0] + vertices[1] * vertices[1] + vertices[2] * vertices[2]);

    if (sf < 0.75) {
      sf -= 1.0;
    }

    *np++ = *vertices++ / sf;
    *np++ = *vertices++ / sf;
    *np++ = *vertices++ / sf;

  }

  return 0;

}

int main(int argc, char *argv[]) {

  nsphere ns;

  char *def_fn = "infile.dat";
  
  char *filename = argc>1 ? argv[1] : def_fn;

  long int num_vertices = argc>2 ? strtol(argv[2], NULL, 10) : 8000;

  double duration;
  double fps;
  
  GLfloat *vertices, *colors, *normals;

  long int filled_vertices;

  int retval;

  transform_pack *tp;

  long int num_frames;

  long int frameno;
  
  long int input_xres, input_yres;
  
  long int xres, yres;

  long int num_pixels;

  size_t img_sz;

  pixel_t *rgb;

  long int vertexno;
  
  filled_vertices = 0;
  
  tp = &(ns.tp);
  
  init_transform_pack(tp);

  if (argc>3) {
    retval = sscanf(argv[3],"%ldx%ld",&input_xres,&input_yres);
  }

  duration = argc>4 ? strtod(argv[4],NULL) : 20.0;

  fps = argc>5 ? strtod(argv[5],NULL) : 60;
  
  xres = input_xres;
  yres = input_yres;

  num_pixels = xres * yres;
  img_sz = num_pixels * sizeof(uint16_t) * 3;

  retval = prepare_vertices(filename, tp, num_vertices, &vertices, &filled_vertices, NULL);
  if (retval==-1) {
    fprintf(stderr, "%s: Trouble with collective calls to prepare vertices.\n", __FUNCTION__);
    return -1;    
  }

  retval = prepare_colors(vertices, filled_vertices, &colors);
  if (retval == -1) {
    fprintf(stderr, "%s: Trouble with color preparation from filled vertices.\n", __FUNCTION__);
    return -1;
  }

  retval = prepare_normals(vertices, filled_vertices, &normals);
  if (retval == -1) {
    fprintf(stderr, "%s: Trouble with normal preparation from filled vertices.\n", __FUNCTION__);
    return -1;
  }

  retval = init(vertices, colors, normals, filled_vertices);

  rgb = malloc(img_sz);
  if (rgb == NULL) {
    perror("malloc");
    return -1;
  }

  num_frames = duration * fps;

  fprintf(stderr, "%s: Rendering %ld frames.\n", __FUNCTION__, num_frames);
  
  for (frameno = 0; frameno < num_frames; frameno++) {

    double spinrate_persec;

    memset(rgb, 0, img_sz);

    spinrate_persec = 0.2;
    
    angle = (2.0 * M_PI * spinrate_persec * duration * frameno) / num_frames;
    
    retval = draw_image(rgb, xres, yres, vertices, colors, filled_vertices, angle);
      
    {

      int out_fd;

      ssize_t bytes_written;

      out_fd = 1;
      
      bytes_written = writefile(out_fd, rgb, img_sz);
      if (bytes_written != img_sz) {
	perror("write");
	return -1;
      }
      
    }
    
  }

  free(rgb);
  
  return 0;

}
