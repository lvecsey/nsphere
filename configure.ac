AC_INIT(nsphere, [0.97], [lvecsey-nsphere@phrasep.com])

AC_PREREQ(2.59)

AM_INIT_AUTOMAKE

AC_PROG_CC

AC_CHECK_HEADER(stdio.h,[AC_DEFINE(HAVE_STDIO_H)],[AC_MSG_ERROR([Sorry, can't do anything for you])])

AC_CONFIG_SRCDIR([src/nsphere.c])

AC_PROG_LIBTOOL

AM_PROG_CC_C_O

AC_FUNC_MALLOC

AC_CHECK_LIB(X11, XOpenDisplay)
AC_CHECK_LIB(GL, glXChooseVisual)

AC_CHECK_HEADERS(malloc.h)
AC_CHECK_HEADERS(assert.h)
AC_CHECK_HEADERS(sys/stat.h)

AC_CHECK_FUNCS(fstat)

AC_CONFIG_FILES([src/library/Makefile src/Makefile src/ppm-out/Makefile Makefile tests/Makefile])

AC_OUTPUT
