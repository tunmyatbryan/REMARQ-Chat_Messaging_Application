#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>   
#include <unistd.h>  
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "DataStructures.h"


int protocolHandler(const char buffer[256]);

/*
	* Takes in the buffer, and handles the REGISTER
	* Creates a "cUsername" and "cPass" from buffer
	* Prints them into a newline in a file called
	* "RegisteredUsers.txt"
	* Returns 1 on success
	* Returns 0 on failure
*/

int ifQuit(const char tempBuffer[256]);

int ServerRegister(const char buffer[256]);

/*
	* Takes in the buffer, and handles the LOGIN
	* Creates a "cUsername" and "cPass" from buffer
	* Compares them to every line until break of
	* "RegisteredUsers.txt"
	* Returns 1 on success
	* Returns 0 on failure
*/
int ServerLogIn(const char buffer[256]);

int ReturnUserID(const char buffer[255]);

/*Pass in a login string, adds credentials to file that stores only Usernames and user ID*/
int UserDataUpdate(const char buffer[256]);

/*takes in a message protocol buffer, returns the sender ID*/
int ReceiverID(const char buffer[255]);

char* getMessage(char buffer[255]);

int send_UserNametxt(int socket);

int GetUserNametxt(int socket);

ULIST* LoadListFromFile();

int UserDisconnect(int i, ULIST *List);

#endif
