
bin_PROGRAMS = nsphere-ppm

AM_CFLAGS = -Wall -I$(top_srcdir)/src/library

nsphere_ppm_SOURCES = ppm-out.c
nsphere_ppm_CFLAGS = $(AM_CFLAGS) 
nsphere_ppm_LDFLAGS = -L$(top_builddir)/src/library/.libs
nsphere_ppm_LDADD = -lm # -lnsphere 
