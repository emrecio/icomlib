TEMPLATE	= app
CONFIG		= qt warn_on debug
HEADERS		= pmainwindow.h \
		  pmenu.h \
		  ptools.h \
		  pstatus.h \
		  pui.h \
		  pkeybd.h \
		  pdisp.h \	
		  pctrl.h \
		  pfileio.h \
		  proedit.h \
		  prodlg.h
SOURCES		= main.cpp \
		  pmainwindow.cpp \
		  pmenu.cpp \
		  ptools.cpp \
		  pstatus.cpp \
		  pui.cpp \
		  pkeybd.cpp \
		  pdisp.cpp \
		  pctrl.cpp \
		  pfileio.cpp \
		  proedit.cpp \
		  prodlg.cpp
TARGET		= qpcr1k
DESTDIR		+= $(HOME)/icomlib/bin
INCLUDEPATH	+= $(HOME)/icomlib/include
#DEFINES		+= DEBUG_VER_ SunOS CDE_STYLE
DEFINES		+= DEBUG_VER_ Linux SGI_STYLE
unix:TMAKE_LIBS	+= -L$(HOME)/icomlib/lib -lpcomm -lpcp
