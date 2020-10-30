# ############################################################
# 02/22/2018		BIANCA		MAKEFILE START
# 02/11/2018		MICHAEL		Added Register and guiTest targets
# 03/01/2018		MICHAEL		Fixed flag for MAIN debugging in separate modules
#
# #############################################################

# Variables
CC     = gcc
DEBUG  = -g -DDEBUG
CFLAGS = -ansi -Wall -std=c99 -c -g
LFLAGS = -ansi -Wall -std=c99
#MAIN   = -DMAIN
#LMAIN = -DLMAIN
#RMAIN = -DRMAIN
GMAIN  = -DGMAIN

# Libs
GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC	= $(GTKINC)
LIBS	= $(GTKLIBS)

OPTS	= -g

# Default target
all: client server2 FileIO PasswordENDE PasswordHide guiTest Remarq


# Object Files
#################### Generate executables  ####################

guiTest: guiTest.o Register.o Login.o GList.o Functions.o DataStructures.o
	$(CC) guiTest.o Register.o Login.o GList.o Functions.o DataStructures.o $(LIBS) $(OPTS) -o guiTest

Register: Register.o Login.o GList.o
	$(CC) Register.o Login.o GList.o $(LIBS) $(OPTS) -o Register

Login: Login.o GList.o
	$(CC) Login.o GList.o $(LIBS) $(OPTS) -o Login

GList: GList.o
	$(CC) GList.o $(LIBS) $(OPTS) -o GList

GListDEBUG: GListDebug.o
	$(CC) GListDebug.o $(LIBS) $(OPTS) -o GListDEBUG

# Simple object files for testing
#
################### Place GUI Targets Here ####################
Register.o: Register.c Register.h Login.h GList.h
	$(CC) -c Register.c $(RMAIN) $(INC) $(OPTS) -o Register.o

guiTest.o: guiTest.c Login.h Register.h GList.h Functions.h DataStructures.h
	$(CC) -c guiTest.c $(INC) $(OPTS) -o guiTest.o

Login.o: Login.c Login.h Register.h GList.h
	$(CC) -c Login.c $(LMAIN) $(INC) $(OPTS) -o Login.o

GList.o: GList.c GList.h Functions.h 
	$(CC) -c GList.c $(MAIN) $(INC) $(OPTS) -o GList.o

GListDebug.o: GList.c GList.h
	$(CC) -c GList.c $(GMAIN) $(INC) $(OPTS) -o GListDebug.o
################################

########## Server Multi Client Targets ######################

ServerMC: ServerMultiClient.o Functions.o DataStructures.o
	$(CC) ServerMultiClient.o Functions.o DataStructures.o $(LFLAGS) -o ServerMC

ServerMultiClient.o: ServerMultiClient.c DataStructures.h Functions.h
	$(CC) $(CFLAGS)  ServerMultiClient.c -o ServerMultiClient.o
	
Functions.o: Functions.c Functions.h DataStructures.h
	$(CC) $(CFLAGS)  Functions.c -o Functions.o
	
DataStructures.o: DataStructures.c DataStructures.h
	$(CC) $(CFLAGS)  DataStructures.c -o DataStructures.o
	
#############################################################

client: client.c
	gcc $(LFLAGS) -D_GNU_SOURCE client.c -o client

server2: server2.c
	gcc $(LFLAGS) server2.c -o server2

FileIO: FileIO.c
	gcc $(LFLAGS) FileIO.c -o FileIO

PasswordHide: PasswordHide.c
	gcc PasswordHide.c -ansi -g -o PasswordHide

PasswordENDE: PasswordENDE.c
	gcc PasswordENDE.c -ansi -g -o PasswordENDE

clean:
	rm -rf *.o $(PROJECT)
	rm -rf client server2 FileIO PasswordENDE PasswordHide Register GList guiTest Login ServerMC

