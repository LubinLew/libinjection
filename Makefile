.DEFAULT_GOAL := all
LIBNAME=injection
OUTDIR=target
MODE=relase
#############################################################
CC=gcc

CFLAGS=-c -Wall -Werror \
  -std=c11 \
  -O3 \
  -pie \
  -fPIE \
  -fPIC \
  -pedantic \
  --param ssp-buffer-size=4\
  -Wextra \
  -Wshadow \
  -Wformat \
  -Wformat-security \
  -Wcast-qual \
  -Wcast-align \
  -Wpointer-arith \
  -Wstack-protector \
  -fstack-protector \
  -D_FORTIFY_SOURCE=2 \
  -D__DEBUG

LFAGS=

#############################################################
INCS=-Ilib/inc

SOURCES = lib/src/libinjection_sqli.c   \
          lib/src/libinjection_html5.c  \
          lib/src/libinjection_xss.c

OBJS=$(SOURCES:%.c=${OUTDIR}/%.o)
#############################################################

.PHONY: all check clean

all: lib${LIBNAME}.a lib${LIBNAME}.so

	
lib${LIBNAME}.a: ${OBJS}
	@ echo "[AR] $@"
	@ ar crs ${OUTDIR}/$@ $^
	@ ls -lh ${OUTDIR}/$@

lib${LIBNAME}.so: ${OBJS}
	@ echo "[SO] $@"
	@ ${CC} -shared -o ${OUTDIR}/$@ $^
	@ ls -lh ${OUTDIR}/$@


check:
	(cd src; ${MAKE} check)

clean:
	@ rm -rf ${OUTDIR}
	@ echo "ALL Clear !!!"

example:
	${CC} -Wall ${INCS} lib/example/sqil_test.c -o sqli_test -L${OUTDIR} -l:lib${LIBNAME}.a

#############################################################
${OUTDIR}/%.o:%.c
	@ mkdir -p $(@D)
	@ echo "[CC] $^"
	@ ${CC} ${CFLAGS} ${INCS} $< -o $@ ${LFLAGS}