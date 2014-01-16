
sbin_PROGRAMS = vhdd

vhdd_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/canMsgApi \
	$(GLIB_CFLAGS)
	
vhdd_SOURCES = \
	main.cpp \
	debug.cpp debug.h \
	WatchDBus.cpp WatchDBus.h \
	ProxyDBus.cpp ProxyDBus.h \
	WatchConnman.cpp WatchConnman.h \
	ConnmanManager.cpp ConnmanManager.h \
	ConnmanService.cpp ConnmanService.h \
	types.cpp types.h \
	IParameter.cpp IParameter.h \
	INetwork.cpp INetwork.h \
	Wlan.cpp Wlan.h \
	Qmi.cpp Qmi.h \
	DataStorage.cpp DataStorage.h \
	IVhda.cpp IVhda.h \
	RssBased.cpp RssBased.h
	
vhdd_LDADD = $(GLIB_LIBS)
# vhdd_LDADD = ../canMsgApi/libCanMsgApi.la