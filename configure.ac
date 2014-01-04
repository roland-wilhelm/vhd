# Prelude
AC_INIT([vhd], [0.1], [rwilhelm@gmx.org])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4]) # needed for Libtool
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR([src/main.cpp])
LT_INIT

# Checks for programs
AC_PROG_CXX

# Checks for libraries.
# Checks for header files.
# Checks for typedefs, structures, and compiler characteristics.
# Checks for library functions.
# Output files.

# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile canMsgApi/Makefile])
AC_OUTPUT

echo "
    vhd $VERSION
    ==============================================

    compiler:                ${CXX}
    cxxflags:                ${CXXFLAGS}
"
