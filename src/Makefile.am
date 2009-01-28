
bin_PROGRAMS = nsphere

AM_CFLAGS = -Wall -I$(top_srcdir)/src/library

nsphere_SOURCES = nsphere.c plot_file.c gen_vertexarray.c prepare_vertices.c spit_prepare.c collection_pack.h collection_pack.c polar_collect.c verbose_read.c polar_collect.h gen_vertexarray.h prepare_vertices.h spit_prepare.h read_status.h verbose_read.h
nsphere_CFLAGS = $(AM_CFLAGS) 
nsphere_LDFLAGS = -L$(top_builddir)/src/library/.libs
nsphere_LDADD = -lglut -lGL -lm -lnsphere 
