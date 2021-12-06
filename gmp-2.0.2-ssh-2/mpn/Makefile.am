## Process this file with automake to produce Makefile.in

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

AUTOMAKE_OPTIONS = 1.0 foreign no-dependencies

SUBDIRS = tests

noinst_LIBRARIES = mpn
noinst_HEADERS = sysdep.h

mpn_SOURCES = mp_bases.c
mpn_LIBADD = @MPN_OBJS@

EXTRA_LIBADD = inlines.o add_n.o addmul_1.o cmp.o divmod_1.o \
  divrem.o divrem_1.o dump.o lshift.o mod_1.o mul.o mul_1.o mul_n.o \
  random2.o rshift.o sqrtrem.o sub_n.o submul_1.o get_str.o set_str.o \
  scan0.o scan1.o popcount.o hamdist.o gcd_1.o pre_mod_1.o perfsqr.o \
  bdivmod.o gcd.o gcdext.o \
  udiv.o udiv_nfp.o udiv_fp.o udiv_qrnnd.o udiv_w_sdiv.o

INCLUDES = -I.. -I$(srcdir)/..

SFLAGS = @SFLAGS@
CPP = @CPP@

SUFFIXES = .S .s
DISTCLEANFILES = @MPN_DISTCLEANFILES@
CLEANFILES = tmp-*.s

.s.o:
	$(CC) -c $(CFLAGS) $(SFLAGS) $<

.S.o:
	$(CPP) $(INCLUDES) $(DEFS) $< | grep -v '^#' >tmp-$*.s
	$(CC) $(CFLAGS) $(SFLAGS) -c tmp-$*.s
	mv tmp-$*.o $@
	rm -f tmp-$*.s
