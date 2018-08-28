TEMPLATE	= lib
CONFIG		= warn_on debug
DESTDIR		= $(SRCDIR)/lib
HEADERS		= pcomm.h \
		  pcrdef.h
SOURCES		= pcomm.cpp
INCLUDEPATH	+= $(SRCDIR)/include
#DEFINES		= DEBUG_VER_
