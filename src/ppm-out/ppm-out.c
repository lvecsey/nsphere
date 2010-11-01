/*

  ppm-out.c, Plots a noise-sphere of the file. (formerly NOISESPH.C)

  The original version of this program was written in Turbo Pascal by
  Rob Rothenburg Walking-Owl <Wlkng...@unix.asb.com>. It was converted
  to Borland C++ by James Pate Williams, Jr. <p...@mindspring.com>,
  then (back) to vanilla C with added options by Rob Walking-Owl.

  This program is public domain.  No copyright is claimed.

  Usage: noisesph file [-3 -b|w -x n -y n -z n -m n -s n -c n]

  This program reads a file of random or pseudo-random data and plots
  a noise sphere of the data. Poor RNGs or sampling methods will show
  clear patterns (definite splotches or spirals).

  The theory behind this is to get a set of 3D polar coordinates from
  the RNG and plot them.  An array is kept of the values, which is
  rotated each time a new byte is read (see the code in the main
  procedure).

  Rather than plot one sphere which can be rotated around any axis,
  it was easier to plot the sphere from three different angles.

  This program is based on a description from the article below.  It
  was proposed as a means of testing pseudo-RNGs:

  Pickover, Clifford A. 1995. "Random number generators: pretty good
    ones are easy to find."  The Visual Computer (1005) 11:369-377.

  See also Pickover, nd., "Keys to Infinity." Chapter 31.

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define STAT
#define SAMPLE unsigned int
#define SHIFTS 8

#ifndef M_PI
#define M_PI 3.14159267
#endif

#ifndef LIGHTGRAY
#define LIGHTGRAY 192
#endif

int width = 1280, height = 1024;
unsigned char *image_data;

#ifdef STAT
unsigned long stats[1000];
#endif
int
  defcolor = LIGHTGRAY,
  triple = 0,
  RZ = 0, RX = 0, RY = 0;

struct Cartesian
{
  double x, y, z;
  unsigned Color;

};

struct Polar
{
  double r, theta, phi;

};

int MidA, MidB, MidC, MidY, Scale;

void InitPoints(int maxw, int maxh) {
  if (triple)
  {
/* Changed to plot in quadrants, since the resolution is slightly larger */
        Scale = maxw / 4;
        MidA = MidC = Scale, MidB = 3 * Scale;
        MidY = maxh / 4;
  } else
  {
        Scale = maxw / 2;
        MidA = Scale;
        MidY = maxh / 2;
  }
  if (MidY < Scale) Scale = MidY;

}

/* Convert degrees to radians */
#define rads(x) (((double)(x * 2)) * M_PI / 360.0)

/* This could be rewritten to scale a number between -1.0 and 1.0 to
   a grayscale or RBG map, depending on the system one is plotting on.
   (Perhaps setting RGB values based on X, Y and Z independently?) */

#define ScaleColor(x) (defcolor)

/*
unsigned int ScaleColor(double x)
{
  if (x<0) return(LIGHTGRAY); else return(WHITE);
}

*/

/* Rotate C along X, Y and Z axes */
void rotate(struct Cartesian *C)
{
  double x,y,z;
  if (RZ)
  {
        z = rads(RZ);
        x = (C->x * cos(z)) + (C->y * sin(z));
        y = (C->y * cos(z)) - (C->x * sin(z));
        C->x = x; C->y = y;
  }
  if (RX)
  {
        x = rads(RX);
        y = (C->y * cos(x)) + (C->z * sin(x));
        z = (C->z * cos(x)) - (C->y * sin(x));
        C->z = z; C->y = y;
  }
  if (RY)
  {
        y = rads(RY);
        x = (C->x * cos(y)) + (C->z * sin(y));
        z = (C->z * cos(y)) - (C->x * sin(y));
        C->z = z; C->x = x;
  }

}

#define project(x) (ceil(Scale * x))

static void putpixel(int x, int y, unsigned color) {

  unsigned char *p;

  assert(x>=0 && y>=0 && x<width && y<height);

  if (x<0 || y<0 || x>width || y>height) return;

  p = image_data + 3 * width * y + 3 * x;

  *p++ = color;
  *p++ = color;
  *p++ = color;

}

/* Projects 3d coordinates to a 2d screen */
void Plot(struct Cartesian C)
{
  rotate(&C);
  putpixel(MidA + project(C.x), MidY - project(C.y), C.Color);
  if (triple)
  {
      putpixel(MidB + project(C.y), MidY - project(C.z), C.Color);
      putpixel(MidC + project(C.z), (3 * MidY) - project(C.x), C.Color);
  }

}

/* Converts 3-d polar coordinates to cartesian coordinates */
void PolarToCartesian(struct Polar P, struct Cartesian *C)
{
  C->x = P.r * sin(P.phi) * cos(P.theta);
  C->y = P.r * sin(P.phi) * sin(P.theta);
  C->z = P.r * cos(P.phi);
/* We can assign colors based on C.x, C.y, C.z,
   or P.r, P.theta / pi or P.phi / (2 * pi) */
  C->Color = ScaleColor(C->z);

}

  double X[3], SRANGE;
  int error, n = 0, tmp,
  lag = 1,
  mode = 0,            // 0 = 8-bit samples, 1 = 16 bit samples
  sr = 0, skip = 0;    // plot only every skipth point
  long int size = -1;  // if size>0, plot only size points
  SAMPLE byte;
  FILE *inpu;
  struct Cartesian C;
  struct Polar P;

void usage(void)
{

  fprintf(stderr, "usage: noisesph [-3 -b|w -x n -y n -z n -m n -s n] file [[opts] file...]\n" \
         "Plots a noise sphere based on the data in file.\n" \
         "-3\tplot projections along (x,y), (z,y) and (x,z) planes\n" \
         "-b\tfile contains 8-bit samples (default)\n" \
         "-w\tfile contains 16-bit samples\n" \
         "-x n\trotate around x-axis n degrees\n" \
         "-y n\trotate around y-axis n degrees\n" \
         "-z n\trotate around z-axis n degrees\n" \
         "-m n\tplot only the first n samples in file\n" \
         "-s n\tplot only every nth point\n" \
         "-c n\tplot in color n (system specific)\n" \
         "-l n\tlag = n (discard every nth sample)\n" \
        );
  exit(EXIT_FAILURE);

}

int plot_file(char* name)
{ int i, j;
  InitPoints(width,height);
  if (!(inpu = fopen(name, "rb"))) {
     fprintf(stderr, "Cannot open file %s\n", name);
     exit(EXIT_FAILURE);
  }
  if (mode) SRANGE = 65536.0; else SRANGE = 256.0;
  for (i = 0; i < 3; i++)
  {
     for (j=0; j<lag; j++) {
         tmp = fgetc(inpu); byte = tmp;
         if (mode)
         {
         // assumes little-endian data files and little-endian machines
                tmp = fgetc(inpu); byte |= ((SAMPLE)(tmp) << SHIFTS);
         }
         X[n] = byte / SRANGE;
         n = (n+1) % 3;
     }
  }
  do
  {
         P.r = sqrt(X[(n + 2) % 3]);
         P.theta = M_PI * X[(n + 1) % 3];
         P.phi = 2 * M_PI * X[n];
#ifdef STAT
         ++stats[ floor(X[n]*10) +
                  (10*floor(X[(n + 1) % 3]*10)) +
                  (100*floor(X[(n + 2) % 3]*10)) ];
#endif
         if (!sr) { PolarToCartesian(P, &C); Plot(C); }
         if (skip) {
           sr++;
           if (sr==skip) sr = 0;
         }

     for (j=0; j<lag; j++) {
         tmp = fgetc(inpu); byte = tmp;
         if ((mode) && (tmp!=EOF))
         {
         // assumes little-endian data files and little-endian machines
                tmp = fgetc(inpu); byte |= ((SAMPLE)(tmp) << SHIFTS);
         }
         X[n] = byte / SRANGE;
         n = (n+1) % 3;
         if (size>0) size = size - (1<<mode);
     }
  }

  while (!feof(inpu) && (tmp!=EOF) && (size)); 

  return 0;

}

static int write_image(int fd, unsigned char *i, int w, int h) {

  int retval, depth = 255, header_len;

  char ppm_header[80];

  header_len = sprintf(ppm_header, "P6\n#generated with nsphere-ppm\n%d %d\n%d\n", width, height, depth);
  if (header_len < 0) {
    fprintf(stderr, "%s: Trouble with call to sprintf.\n", __FUNCTION__);
    return -1;
  }
  retval = write(fd, ppm_header, header_len);
  if (retval != header_len) {
    fprintf(stderr, "%s: Trouble writing ppm header.\n", __FUNCTION__);
    return -1;
  }

  retval = write(fd, i, 3*width*height);
  if (retval != 3*width*height) {
    printf("%s: Partial write for image data.\n", __FUNCTION__);
    return -1;
  }

  return 0;

}

int main(int argc, char* argv[]) { 

  int i,fd,retval;
#ifdef STAT
  for (i=0;i<1000;i++) stats[i]=0;
#endif
  if (argc < 2) {
     usage();
  }

  image_data = malloc(3*width*height);
  if (image_data==NULL) {
    fprintf(stderr, "Problem with allocating image data buffer.\n");
    exit(EXIT_FAILURE);
  }

  fd = open("out1.ppm", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  if(fd==-1) {
    fprintf(stderr, "Problem with open for write. retval=%d\n", fd);
    exit(EXIT_FAILURE);
  }

  if (argc > 1)
  for (i = 1; ((i<argc)); i++)
  {
        if (argv[i][0] != '-') plot_file(argv[i]);
        else if (strcmp(argv[i], "-w") == 0) mode = 1;
        else if (strcmp(argv[i], "-b") == 0) mode = 0;
        else if (strcmp(argv[i], "-3") == 0) triple = 1;
        else if (strcmp(argv[i], "-x") == 0) RX = atoi(argv[++i]);
        else if (strcmp(argv[i], "-y") == 0) RY = atoi(argv[++i]);
        else if (strcmp(argv[i], "-z") == 0) RZ = atoi(argv[++i]);
        else if (strcmp(argv[i], "-s") == 0) skip = atoi(argv[++i]);
        else if (strcmp(argv[i], "-m") == 0) size = atol(argv[++i]);
        else if (strcmp(argv[i], "-c") == 0) defcolor = atol(argv[++i]);
        else if (strcmp(argv[i], "-l") == 0) lag = atoi(argv[++i]);
        else {
          printf("Unrecognized option: %s\n", argv[i]);
          usage();
        }
  }
  retval = write_image(fd, image_data, width, height);
  if (retval==-1) {
    fprintf(stderr, "Problem with writing image data.\n");
    exit(EXIT_FAILURE);
  }
  close(fd);
#ifdef STAT
  for(i=0;i<1000;i++)
    printf("[%d] = %d\n", i, stats[i]);
#endif
  exit(EXIT_SUCCESS);

}
