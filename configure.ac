AC_INIT(smessaging, 1.0)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/serdes.c])
AC_CONFIG_HEADERS([config/config.h])
AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_CC
AC_PROG_RANLIB

# Configures Ulapi.
ACX_ULAPI

# Configures Doxygen.
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([smessaging],[Doxyfile],[doc])

AC_CONFIG_FILES([Makefile bin/Makefile lib/Makefile])

AC_OUTPUT
