# This file was generated automatically by configure.  Do not edit.
VPATH = .
links =  add.c cmp.c gcd.c mul.c pow_ui.c powm.c sqrtrem.c sub.c
host_alias = i386-unknown-freebsd2.1.0
host_cpu = i386
host_vendor = unknown
host_os = freebsd2.1.0
host_canonical = i386-unknown-freebsd2.1.0
target_alias = i386-unknown-freebsd2.1.0
target_cpu = i386
target_vendor = unknown
target_os = freebsd2.1.0
target_canonical = i386-unknown-freebsd2.1.0
build_alias = i386-unknown-freebsd2.1.0
build_cpu = i386
build_vendor = unknown
build_os = freebsd2.1.0
build_canonical = i386-unknown-freebsd2.1.0
# Makefile for GNU MP/mpbsd functions
# Copyright (C) 1991, 1993, 1994, 1996 Free Software Foundation, Inc.

# This file is part of the GNU MP Library.

# The GNU MP Library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.

# The GNU MP Library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
# License for more details.

# You should have received a copy of the GNU Library General Public License
# along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA.

srcdir = .

CC = gcc -O2

# If you cross compile on a machine with the same sizes of the integral
# types ("int", "long int", "short int", and "char") define this as the
# local compiler.  Otherwise, you need to look for the uses of LOCAL_CC below,
# and handle those cases manually.
LOCAL_CC = $(CC)
CFLAGS = -g -O
AR = ar
SHELL = /bin/sh

#### host and target specific makefile fragments come in here.
###

MPBSD_LINKS = add.c cmp.c gcd.c mul.c pow_ui.c  powm.c sqrtrem.c sub.c
MPBSD_SRCS = itom.c mdiv.c mfree.c min.c mout.c move.c mtox.c sdiv.c xtom.c \
  realloc.c $(MPBSD_LINKS)

MPBSD_OBJS = itom.o mdiv.o mfree.o min.o mout.o move.o mtox.o sdiv.o xtom.o \
  realloc.o add.o cmp.o gcd.o mul.o pow_ui.o powm.o sqrtrem.o sub.o

INCLUDES = -I. -I.. -I$(srcdir)/../mpz -I../mpn -I$(srcdir)/..

libmpbsd.a: Makefile.in $(MPBSD_OBJS)
	rm -f $@
	$(AR) cr $@ $(MPBSD_OBJS)

.c.o:
	$(CC) -c $(INCLUDES) -DBERKELEY_MP $(CFLAGS) $(XCFLAGS) $<

check:
	true

clean mostlyclean:
	rm -f *.o libmpbsd.a
distclean maintainer-clean: clean
	rm -f Makefile config.status $(MPBSD_LINKS)

Makefile: $(srcdir)/Makefile.in
	$(SHELL) ./config.status

H = $(srcdir)/../gmp.h $(srcdir)/../gmp-impl.h ../mpn/gmp-mparam.h
L = $(srcdir)/../longlong.h

itom.o: $(srcdir)/itom.c ../mp.h $(H)
mdiv.o: $(srcdir)/mdiv.c ../mp.h $(H) $(L) $(srcdir)/../mpz/dmincl.c
mfree.o: $(srcdir)/mfree.c ../mp.h $(H)
min.o: $(srcdir)/min.c ../mp.h $(H)
mout.o: $(srcdir)/mout.c ../mp.h $(H)
move.o: $(srcdir)/move.c ../mp.h $(H)
mtox.o: $(srcdir)/mtox.c ../mp.h $(H)
sdiv.o: $(srcdir)/sdiv.c ../mp.h $(H) $(L)
xtom.o: $(srcdir)/xtom.c ../mp.h $(H)
add.o: $(srcdir)/../mpz/add.c $(H)
cmp.o: $(srcdir)/../mpz/cmp.c $(H)
gcd.o: $(srcdir)/../mpz/gcd.c $(H) $(L)
mul.o: $(srcdir)/../mpz/mul.c $(H)
pow_ui.o: $(srcdir)/../mpz/pow_ui.c $(H) $(L)
powm.o: $(srcdir)/../mpz/powm.c $(H) $(L)
realloc.o: $(srcdir)/realloc.c $(H)
sqrtrem.o: $(srcdir)/../mpz/sqrtrem.c $(H)
sub.o: $(srcdir)/../mpz/sub.c $(H)
