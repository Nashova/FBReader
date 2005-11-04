include $(ROOTDIR)/makefiles/arch/dummy.mk

CC = gcc
AR = ar rsu
LD = g++

DEPGEN = $(CC) -MM
CFLAGS = -pipe $(DUMMY_SECTIONS) -fno-exceptions -fno-rtti -Wall -Wno-ctor-dtor-privacy -W
RM = rm -rvf
RM_QUIET = rm -rf
GTKINCLUDE = -I/usr/include/libxml2 -I/usr/include/libglade-2.0 -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/X11R6/include -I/usr/include/atk-1.0 -I/usr/include/pango-1.0 -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/hildon-lgpl -I/usr/include/dbus-1.0
UILIBS = -lz -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lm -lgmodule-2.0 -ldl -lgthread-2.0 -lhildonlgpl -lhildonwidgets -losso
EXPATLIBS = -lexpat
ENCALIBS = /usr/lib/libenca.a
