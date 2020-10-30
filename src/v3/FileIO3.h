#ifndef FILEIO3_H
#define FILEIO3_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "struct.h"

#define SLEN 20

/* function declarations */

void PrintLoginMenu();


int ServerRegister(const char cUser[SLEN],const char cPass[SLEN]);

/*char* ClientLogIn(const char cUser[SLEN], const char cPass[SLEN], char *LoginProtocol);*/

int ServerLogIn(const char cUser[SLEN], const char cPass[SLEN]);

char* FL_FileLoad(USER *u);

void FL_FileSave(USER *u, const char *s);

int getch();

void encrypt(char password[],int key);

void decrypt(char password[],int key);
/* end of function declarations */

#endif
