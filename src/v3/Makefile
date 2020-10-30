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
all: LoginDemoClient LoginDemoServer ClientTest ServerTest


# Object Files
FileIO3.o: FileIO3.h FileIO3.c
	gcc $(CFLAGS) FileIO3.c -o FileIO3.o
	
client3.o: client3.c FileIO3.h
	gcc $(CFLAGS) -D_GNU_SOURCE client3.c -o client3.o

server3.o: server3.c FileIO3.h
	gcc $(CFLAGS) server3.c -o server3.o
	
client4.o: Client4.c FileIO3.h
	gcc $(CFLAGS) -D_GNU_SOURCE Client4.c -o client4.o

server4.o: Server4.c FileIO3.h
	gcc $(CFLAGS) -D_GNU_SOURCE Server4.c -o server4.o
#################### Generate executables  ####################


# Simple object files for testing
LoginDemoClient: client3.o FileIO3.o
	gcc $(LFLAGS) client3.o FileIO3.o -o LoginDemoClient

ClientTest: client4.o FileIO3.o
	gcc $(LFLAGS) client4.o FileIO3.o -o ClientTest

LoginDemoServer: server3.o FileIO3.o
	gcc $(LFLAGS) server3.o FileIO3.o -o LoginDemoServer

ServerTest: server4.o FileIO3.o
	gcc $(LFLAGS) server4.o FileIO3.o -o ServerTest

serverSend: serverSend.c
	gcc $(LFLAGS) serverSend.c -o serverSend

clientRec: clientRec.c
	gcc $(LFLAGS) clientRec.c -o clientRec

servertxtSend: servertxtSend.c
	gcc $(LFLAGS) servertxtSend.c -o servertxtSend

clienttxtRec: clienttxtRec.c
	gcc $(LFLAGS) clienttxtRec.c -o clienttxtRec

clean:
	rm -rf *.o $(PROJECT)
	rm -rf LoginDemoClient ClientTest LoginDemoServer ServerTest RegisteredUsers.txt serverSend clientRec servertxtSend clienttxtRec
	rm -rf *.c~ *.h~
	rm -rf USERNAME_Client.txt
