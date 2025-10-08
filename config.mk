# slock version
VERSION = 1.6

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

PKG_CONFIG = pkg-config

# Uncomment for BSD
#BSD=-D_BSD_SOURCE

# Uncomment for NetBSD
#NETBSD=-D_NETBSD_SOURCE

# Optionally slock can be compiled with PAM authentication support
#PAMLIB=-lpam
#PAM=-DHAVE_PAM=1

# Optionally slock can be compiled with DPMS support
#DPMS=-DHAVE_DPMS=1

# Optionally slock can be compiled with imlib2 support to load e.g. .png or jpg images
# to be used as a background. Note that if this is not included slock can still read a
# farbfeld image without the use of this library. Screenshot of the current desktop is
# also done without the use of this library.
#IMLIB=-lImlib2
#IM=-DHAVE_IMLIB=1

# Optionally slock can be compiled with threading support, for auto-timeout functionality
#THREAD=-DHAVE_PTHREAD=1

CONFIG = `$(PKG_CONFIG) --libs libconfig`

# includes and libs
INCS = -I. -I/usr/include -I${X11INC}
LIBS = -L/usr/lib -lc -lcrypt -L${X11LIB} -lX11 -lm -lXext -lXrandr ${CONFIG} ${PAMLIB} ${IMLIB}

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_DEFAULT_SOURCE -DHAVE_SHADOW_H ${DPMS} ${PAM} ${IM} ${BSD} ${THREAD} ${NETBSD}
CFLAGS = -std=c99 -pedantic -Wall -Wno-unused-function -Os ${INCS} ${CPPFLAGS}
LDFLAGS = -s ${LIBS}
COMPATSRC = explicit_bzero.c

# On OpenBSD and Darwin remove -lcrypt from LIBS
#LIBS = -L/usr/lib -lc -L${X11LIB} -lX11 -lXext -lXrandr
# On *BSD remove -DHAVE_SHADOW_H from CPPFLAGS
# On NetBSD add -D_NETBSD_SOURCE to CPPFLAGS
#CPPFLAGS = -DVERSION=\"${VERSION}\" -D_BSD_SOURCE -D_NETBSD_SOURCE
# On OpenBSD set COMPATSRC to empty
#COMPATSRC =
