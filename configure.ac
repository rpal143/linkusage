#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([linkusage], [1.0], [rahulbedarkar89@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/linkusage.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNC([getline])
AC_CHECK_FUNC([sleep])

AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT
