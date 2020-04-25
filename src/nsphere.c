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
#include <GL/glut.h>

#include <math.h>

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

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLfloat angle = 0.0;

static GLint nsphere1;

uint64_t max_frames;

uint64_t count = 0;

capframe cf;

void draw(void) {

  GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat blue[4] = {0.0, 0.0, 1.0, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glRotatef(view_rotx, 1.0, 0.0, 0.0);
  glRotatef(view_roty, 0.0, 1.0, 0.0);
  glRotatef(view_rotz, 0.0, 0.0, 1.0);

  glPushMatrix();
  glRotatef(angle, 0.0, 1.0, 0.0);
  glCallList(nsphere1);
  glPopMatrix();

  glPopMatrix();

  if (cf.state == CF_ONESHOT) {
    process_capframe(&cf);
  }
  
  glutSwapBuffers();

  count++;
  if (count == max_frames) {
    exit(EXIT_SUCCESS);
  }

}

static void idle(void) {

  //angle += 2.0;
  angle += 0.02;
  glutPostRedisplay();

}

void key(unsigned char k, int x, int y) {

  switch (k) {
  case 'z':
    view_rotz += 5.0;
    break;
  case 'Z':
    view_rotz -= 5.0;
    break;
  case 27:
    exit(EXIT_SUCCESS);
    break;
  default:
    return;
  }

  glutPostRedisplay();

}

void special(int k, int x, int y) {

  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    view_roty += 5.0;
    break;
  case GLUT_KEY_RIGHT:
    view_roty -= 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

void reshape(int width, int height) {

  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 3.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -7.0);

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

void visible(int vis) {

  glutIdleFunc( vis == GLUT_VISIBLE ? idle : NULL );
  
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

  GLfloat *vertices, *colors, *normals;

  long int filled_vertices;

  int retval;

  transform_pack *tp;

  long int num_frames;

  long int xres, yres;

  filled_vertices = 0;
  
  tp = &(ns.tp);
  
  init_transform_pack(tp);

  if (getenv("NSPHERE_SPIT") != NULL || getenv("NSPHERE_CSV") != NULL) {
    
    csv_prepare(filename, tp, num_vertices, &vertices, &filled_vertices, NULL);
    
    return 0;

  }

  xres = 1280;
  yres = 720;
  
  if (getenv("CAPFRAME") != NULL) {

    cf = init_capframe(xres, yres, "/tmp/output_nsphere");
    
  }
  
  glutInit(&argc, argv);
  num_frames = argc>3 ? strtol(argv[3],NULL,10) : 0;
  max_frames = num_frames;
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowSize(xres, yres);
  
  glutCreateWindow("nsphere");

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

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutVisibilityFunc(visible);

  glutMainLoop();

  return 0;

}
