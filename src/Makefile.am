
sbin_PROGRAMS = vhdd

vhdd_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/canMsgApi \
	$(GLIB_CFLAGS)
	
vhdd_SOURCES = \
	main.cpp \
	debug.cpp debug.h \
	ProxyDBus.cpp ProxyDBus.h
	
vhdd_LDADD = $(GLIB_LIBS)
# vhdd_LDADD = ../canMsgApi/libCanMsgApi.la