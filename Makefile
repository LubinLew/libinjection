.DEFAULT_GOAL := all
LIBNAME=libinjection
OUTDIR=target
MODE=relase
#############################################################
CC=gcc

CFLAGS=-Wall -WError \
  -O3 \
  -pie \
  -fPIE \
  -fPIC \
  -ansi \
  -pedantic \
  -param \
  -Wextra \
  -Wshadow \
  -Wformat \
  -Wformat-security \
  -Wcast-qual \
  -Wcast-align \
  -Wpointer-arith \
  -Wstack-protector \
  -fstack-protector \
  -ssp-buffer-size=4 \
  -D_FORTIFY_SOURCE=2

LFAGS=

#############################################################
INCS=-Ilib/inc

SOURCES = lib/src/libinjection_sqli.c   \
          lib/src/libinjection_html5.c  \
          lib/src/libinjection_xss.c

OBJS=$(SOURCES:%.c=${OUTDIR}/%.o)
#############################################################

.PHONY: all check clean

all: ${LIBNAME}.a ${LIBNAME}.so

	
${LIBNAME}.a: ${OBJS}
	@ echo "[AR] $@"
	@ ar crs ${OUTDIR}/$@ $^
	@ ls -lh ${OUTDIR}/$@

${LIBNAME}.so: ${OBJS}
	@ echo "[SO] $@"
	@ ${CC} -shared -o ${OUTDIR}/$@ $^
	@ ls -lh ${OUTDIR}/$@


check:
	(cd src; ${MAKE} check)

clean:
	@ rm -rf ${OUTDIR}
	@ echo "ALL Clear !!!"

#############################################################
${OUTDIR}/%.o:%.c
	@ mkdir -p $(@D)
	@ echo "[CC] $^"
	@ ${CC} ${CFLAGS} ${INCS} $< -o $@ ${LFLAGS}