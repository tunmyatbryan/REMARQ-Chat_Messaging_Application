# ############################################################
# 02/22/2018		BIANCA		MAKEFILE START
#
# #############################################################

# Variables
CC     = gcc
DEBUG  = -g -DDEBUG
CFLAGS = -ansi -Wall -std=c99 -c -g
LFLAGS = -ansi -Wall -std=c99

# Libs
GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC	= $(GTKINC)
LIBS	= $(GTKLIBS)

OPTS	= -g

# Default target
all: client server


# Object Files
#################### Generate executables  ####################

clean:
	rm -rf *.o $(PROJECT)
