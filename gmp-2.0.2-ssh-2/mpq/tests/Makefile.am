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

INCLUDES = -I../.. -I$(srcdir)/../.. -I../../mpn -I$(srcdir)/../../mpn
LDADD = ../../libgmp.a

TESTS = t-cmp t-cmp_ui t-get_d

EXTRA_PROGRAMS = t-cmp t-cmp_ui t-get_d

t_cmp_SOURCES = t-cmp.c
t_cmp_ui_SOURCES = t-cmp_ui.c
t_get_d_SOURCES = t-get_d.c

CLEANFILES = $(EXTRA_PROGRAMS)
