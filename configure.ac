# Require a recent version of autotools
AC_PREREQ(2.59)

# Auto conf init
AC_INIT([sudoku],[1.0],[seblu@seblu.net], [sudoku])

# Define configure generator directory
AC_CONFIG_AUX_DIR([build])

# Auto Make init
AM_INIT_AUTOMAKE([foreign dist-bzip2 no-dist-gzip])

# Check platform
AC_CANONICAL_HOST

CXXFLAGS='-Wall -W -ansi -pedantic -D_XOPEN_SOURCE=600'

# Check for C++ compiler
AC_LANG([C++])
AC_PROG_CXX

# Check for Make
AC_PROG_MAKE_SET

# check for ranlib
AC_PROG_RANLIB

# Check for lib efence
AC_CHECK_LIB([efence], [malloc], [EFENCELIB=-lefence])
AC_SUBST([EFENCELIB])

# Checks for library functions.

# Checks for typedefs, structures, and compiler characteristics.

# Check for headers

dnl Memo:
dnl AC ARG WITH(package, help-string, [action-if-given], [action-if-not-given])


AC_ARG_WITH([noerror],
  [AS_HELP_STRING([--with-noerror], [Warning dont create compilation error])],
  [dnl action-if-given
       true
  ],
  [dnl action-if-not-given
       CXXFLAGS="$CFLAGS -Werror"
  ]
)

AC_ARG_WITH([debug],
  [AS_HELP_STRING([--with-debug], [use -g and don't use -DNDEBUG -O3])],
  [dnl action-if-given
       CXXFLAGS="$CFLAGS -g"
  ],
  [dnl action-if-not-given
      CXXFLAGS="$CFLAGS -DNDEBUG -O3"
  ]
)

AC_ARG_WITH([efence],
  [AS_HELP_STRING([--with-efence], [link with lib efence])],
  [dnl action-if-given
       LDFLAGS="$LDFLAGS -lefence"
       CXXFLAGS="$CXXFLAGS -include stdlib.h -include efence.h"
  ],
  [dnl action-if-not-given
       test "$dynamic" = "true" || LDFLAGS="$LDFLAGS -static"
  ]
)

AC_SUBST([CXXFLAGS])
AC_SUBST([LDFLAGS])

# define Autoconf config header
AC_CONFIG_HEADERS([config.h])

# define Autoconf config files
AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT