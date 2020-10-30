#ifndef FILEIO_H
#define FILEIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SLEN 20

/* function declarations */
/*int main();*/

void PrintLoginMenu();

/*char* ClientRegister(const char cUser[SLEN], const char cPass[SLEN], char *RegisterProtocol);*/

int ServerRegister(const char cUser[SLEN],const char cPass[SLEN]);

/*char* ClientLogIn(const char cUser[SLEN], const char cPass[SLEN], char *LoginProtocol);*/

int ServerLogIn(const char cUser[SLEN], const char cPass[SLEN]);

/* end of function declarations */

#endif
