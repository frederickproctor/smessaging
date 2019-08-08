AM_CPPFLAGS = -I@ULAPI_DIR@/include

bin_PROGRAMS = serdes_gen serdes_encode serdes_decode nodemgr reporttest querytest

serdes_gen_SOURCES = ../src/serdes_gen.c
serdes_gen_DEPENDENCIES = ../lib/libsmsg.a
serdes_gen_LDADD = -L../lib -lsmsg

serdes_encode_SOURCES = ../src/serdes_encode.c
serdes_encode_DEPENDENCIES = ../lib/libsmsg.a
serdes_encode_LDADD = -L../lib -lsmsg @ULAPI_LIBS@

serdes_decode_SOURCES = ../src/serdes_decode.c
serdes_decode_DEPENDENCIES = ../lib/libsmsg.a
serdes_decode_LDADD = -L../lib -lsmsg @ULAPI_LIBS@

nodemgr_SOURCES = ../src/nodemgr.c
nodemgr_DEPENDENCIES = ../lib/libsmsg.a
nodemgr_LDADD = -L../lib -lsmsg @ULAPI_LIBS@

reporttest_SOURCES = ../src/reporttest.c
reporttest_DEPENDENCIES = ../lib/libsmsg.a
reporttest_LDADD = -L../lib -lsmsg @ULAPI_LIBS@

querytest_SOURCES = ../src/querytest.c
querytest_DEPENDENCIES = ../lib/libsmsg.a
querytest_LDADD = -L../lib -lsmsg @ULAPI_LIBS@
