
bin_PROGRAMS = nsphere nsphere-ffmpeg

AM_CFLAGS = -Wall -O3 -g -pg -I$(top_srcdir)/src/library

nsphere_SOURCES = nsphere.c plot_file.c gen_vertexarray.c prepare_vertices.c csv_prepare.c collection_pack.h collection_pack.c polar_collect.c verbose_read.c polar_collect.h gen_vertexarray.h prepare_vertices.h spit_prepare.h read_status.h verbose_read.h capframe.c
nsphere_CFLAGS = $(AM_CFLAGS) 
nsphere_LDFLAGS = -L$(top_builddir)/src/library/.libs
nsphere_LDADD = -lglut -lGL -lm -lnsphere 

nsphere_ffmpeg_SOURCES = plot_file.c gen_vertexarray.c prepare_vertices.c csv_prepare.c collection_pack.h collection_pack.c polar_collect.c verbose_read.c polar_collect.h gen_vertexarray.h prepare_vertices.h spit_prepare.h read_status.h verbose_read.h capframe.c writefile.c nsphere-ffmpeg.c
nsphere_ffmpeg_CFLAGS = $(AM_CFLAGS) 
nsphere_ffmpeg_LDFLAGS = -L$(top_builddir)/src/library/.libs
nsphere_ffmpeg_LDADD = -lglut -lGL -lm -lnsphere 
