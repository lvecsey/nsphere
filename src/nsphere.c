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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "polar.h"
#include "cartesian.h"

#include "coordinate_conversion.h"

#include "nsphere.h"

#include "collection_pack.h"

#include "gen_vertexarray.h"

#include "plot_file.h"

#include "transform_pack.h"

#include "prepare_vertices.h"

#include "spit_prepare.h"

#include "verbose_read.h"

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLfloat angle = 0.0;

static GLint nsphere1;

static GLuint limit;
static GLuint count = 1;

static void draw(void) {

  static GLfloat white[4] =
    {1.0, 1.0, 1.0, 1.0};
  static GLfloat blue[4] =
    {0.0, 0.0, 1.0, 1.0};

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

  glutSwapBuffers();

  count++;
  if (count == limit) {
    exit(0);
  }
}

static void idle(void) {

  //angle += 2.0;
  angle += 0.02;
  glutPostRedisplay();

}

/* change view angle, exit upon ESC */
/* ARGSUSED1 */
static void
key(unsigned char k, int x, int y)
{
  switch (k) {
  case 'z':
    view_rotz += 5.0;
    break;
  case 'Z':
    view_rotz -= 5.0;
    break;
  case 27:  /* Escape */
    exit(0);
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* change view angle */
/* ARGSUSED1 */
static void
special(int k, int x, int y)
{
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

/* new window size or exposure */
static void
reshape(int width, int height)
{
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 3.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -7.0);
}

static void init(GLfloat *vertices, GLfloat *colors, int filled_vertices)
{
  static GLfloat pos1[4] =
  {2.5, 2.5, -5.0, 0.0};
  static GLfloat pos2[4] =
  {-2.5, 2.5, -3.0, 0.0};
  static GLfloat red[4] =
  {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] =
  {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4] =
    {0.2, 0.2, 1.0, 1.0};

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

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glColorPointer(4, GL_FLOAT, 0, colors);

  glDrawArrays(GL_POINTS, 3, filled_vertices);  

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glEndList();

  glEnable(GL_NORMALIZE);
}

void 
visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(idle);
  else
    glutIdleFunc(NULL);
}

static int prepare_colors(GLfloat *vertices, int filled_vertices, GLfloat **colors) {

  GLfloat *c, *end;

  assert(vertices!=NULL && filled_vertices > 0);

  assert(colors!=NULL);

  c = malloc(4 * sizeof(GLfloat) * filled_vertices);
  if (c == NULL) {
    fprintf(stderr, "%s: Trouble with malloc for color array.\n", __FUNCTION__);
    return -1;
  }

  *colors = c;
  
  for (end = c + 4 * filled_vertices; c < end; ) {

    *c++ = *vertices++;
    *c++ = *vertices++;
    *c++ = *vertices++;
    *c++ = 1.0;

  }

  return 0;

}

int main(int argc, char *argv[]) {

  struct nsphere n;

  char *filename = argc>1 ? argv[1] : "infile.dat";

  int num_vertices = argc>2 ? strtol(argv[2], NULL, 10) : 8000;

  GLfloat *vertices, *colors;

  int filled_vertices = 0;

  int retval;

  init_transform_pack(&n.t);

  if (getenv("NSPHERE_SPIT") != NULL) {
    
    spit_prepare(filename, &n.t, num_vertices, &vertices, &filled_vertices, NULL);
    
    return 0;

  }

  glutInit(&argc, argv);
  if (argc > 1) {
    /* do 'n' frames then exit */
    limit = atoi(argv[1]) + 1;
  } else {
    limit = 0;
  }
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutCreateWindow("nsphere");

  retval = prepare_vertices(filename, &n.t, num_vertices, &vertices, &filled_vertices, NULL);
  if (retval==-1) {
    fprintf(stderr, "%s: Trouble with collective calls to prepare vertices.\n", __FUNCTION__);
    return -1;    
  }

  retval = prepare_colors(vertices, filled_vertices, &colors);
  if (retval == -1) {
    fprintf(stderr, "%s: Trouble with color preparation from filled vertices.\n", __FUNCTION__);
    return -1;
  }

  init(vertices, colors, filled_vertices);

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutVisibilityFunc(visible);

  glutMainLoop();

  return 0;             /* ANSI C requires main to return int. */
}
