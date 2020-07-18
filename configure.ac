AC_INIT([cpu-c-sim], [0.0], [@OdnetninI])
AC_PREREQ([2.69])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR(build-aux)
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T
AC_TYPE_INT64_T
AC_TYPE_INT32_T
AC_TYPE_INT16_T
AC_TYPE_INT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/cpu/Makefile])
AC_OUTPUT
