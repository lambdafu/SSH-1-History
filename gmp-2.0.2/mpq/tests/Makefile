# This file was generated automatically by configure.  Do not edit.
VPATH = .
links = 
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
# Makefile for mpq/tests for GNU MP

srcdir = .

CC = gcc -O2

TEST_LIBS = ../../libgmp.a
INCLUDES = -I../../mpn -I$(srcdir)/../..
CFLAGS = -g -O

.c.o:
	$(CC) -c $(INCLUDES) $(CFLAGS) $(XCFLAGS) $<

TEST_SRCS = t-cmp.c t-cmp_ui.c t-get_d.c
TEST_OBJS = t-cmp.o t-cmp_ui.o t-get_d.o
TESTS = t-cmp t-cmp_ui t-get_d

check: Makefile st-cmp st-cmp_ui st-get_d
	@echo "The tests passed."

st-cmp: t-cmp
	./t-cmp
	touch $@
st-cmp_ui: t-cmp_ui
	./t-cmp_ui
	touch $@
st-get_d: t-get_d
	./t-get_d
	touch $@

t-cmp: t-cmp.o $(TEST_LIBS)
	$(CC) -o $@ $@.o $(TEST_LIBS) $(CFLAGS)
t-cmp_ui: t-cmp_ui.o $(TEST_LIBS)
	$(CC) -o $@ $@.o $(TEST_LIBS) $(CFLAGS)
t-get_d: t-get_d.o $(TEST_LIBS)
	$(CC) -o $@ $@.o $(TEST_LIBS) $(CFLAGS)

t-cmp.o: $(srcdir)/t-cmp.c
t-cmp_ui.o: $(srcdir)/t-cmp_ui.c
t-get_d.o: $(srcdir)/t-get_d.c

clean mostlyclean:
	rm -f *.o st-* $(TESTS)
distclean maintainer-clean: clean
	rm -f Makefile config.status

Makefile: $(srcdir)/Makefile.in
	$(SHELL) ./config.status
