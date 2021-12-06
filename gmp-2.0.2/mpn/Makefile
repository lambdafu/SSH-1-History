# This file was generated automatically by configure.  Do not edit.
VPATH = .
links = sysdep.h  inlines.c add_n.S addmul_1.S cmp.c divmod_1.c divrem.c divrem_1.c dump.c lshift.S mod_1.c mul.c mul_1.S mul_n.c random2.c rshift.S sqrtrem.c sub_n.S submul_1.S get_str.c set_str.c scan0.c scan1.c popcount.c hamdist.c gcd_1.c pre_mod_1.c perfsqr.c bdivmod.c gcd.c gcdext.c gmp-mparam.h
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
target_makefile_frag = config/t-oldgas
# Makefile for GNU MP/mpn functions
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

MPN_OBJECTS = inlines.o add_n.o addmul_1.o cmp.o divmod_1.o divrem.o divrem_1.o dump.o lshift.o mod_1.o mul.o mul_1.o mul_n.o random2.o rshift.o sqrtrem.o sub_n.o submul_1.o get_str.o set_str.o scan0.o scan1.o popcount.o hamdist.o gcd_1.o pre_mod_1.o perfsqr.o bdivmod.o gcd.o gcdext.o
MPN_LINKS =sysdep.h  inlines.c add_n.S addmul_1.S cmp.c divmod_1.c divrem.c divrem_1.c dump.c lshift.S mod_1.c mul.c mul_1.S mul_n.c random2.c rshift.S sqrtrem.c sub_n.S submul_1.S get_str.c set_str.c scan0.c scan1.c popcount.c hamdist.c gcd_1.c pre_mod_1.c perfsqr.c bdivmod.c gcd.c gcdext.c gmp-mparam.h
CC = gcc -O2
CPP = $(CC) -E
CFLAGS = -g -O
INCLUDES = -I. -I.. -I$(srcdir) -I$(srcdir)/..
AR = ar
AR_FLAGS = rc
SFLAGS=

#### host and target specific makefile fragments come in here.
SFLAGS=-DBROKEN_ALIGN
###

libmpn.a: Makefile mp_bases.o $(MPN_OBJECTS)
	rm -f $@
	$(AR) $(AR_FLAGS) $@ mp_bases.o $(MPN_OBJECTS)

.SUFFIXES: .c .s .S

.c.o:
	$(CC) -c $(INCLUDES) $(CFLAGS) $(XCFLAGS) $<

.s.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(CPP) $(SFLAGS) $(INCLUDES) $(CFLAGS) $< | grep -v '^#' >tmp-$*.s
	$(CC) -c tmp-$*.s -o $@
	rm -f tmp-$*.s

clean mostlyclean:
	rm -f *.o tmp-* libmpn.a
	#-cd tests; $(MAKE) $@
distclean maintainer-clean: clean
	rm -f asm-syntax.h Makefile config.status $(MPN_LINKS)
	-cd tests; $(MAKE) $@

Makefile: $(srcdir)/Makefile.in
	$(SHELL) ./config.status


# Maybe configure could add dependencies here..?

H = $(srcdir)/../gmp.h $(srcdir)/../gmp-impl.h gmp-mparam.h
L = $(srcdir)/../longlong.h

mp_bases.o: $(srcdir)/mp_bases.c $(H)
bdivmod.o: bdivmod.c $(H) $(L)
cmp.o: cmp.c $(H)
divmod_1.o: divmod_1.c $(H) $(L)
divrem.o: divrem.c $(H) $(L)
divrem_1.o: divrem_1.c $(H) $(L)
dump.o: dump.c $(H)
gcd.o: gcd.c $(H) $(L)
gcd_1.o: gcd_1.c $(H) $(L)
gcdext.o: gcdext.c $(H) $(L)
get_str.o: get_str.c $(H) $(L)
hamdist.o: hamdist.c $(H)
inlines.o: inlines.c $(srcdir)/../gmp.h
mod_1.o: mod_1.c $(H) $(L)
mul.o: mul.c $(H)
mul_n.o: mul_n.c $(H)
perfsqr.o: perfsqr.c $(H) $(L)
popcount.o: popcount.c $(H)
pre_mod_1.o: pre_mod_1.c $(H) $(L)
random2.o: random2.c $(H)
scan0.o: scan0.c $(H) $(L)
scan1.o: scan1.c $(H) $(L)
set_str.o: set_str.c $(H)
sqrtrem.o: sqrtrem.c $(H) $(L)
