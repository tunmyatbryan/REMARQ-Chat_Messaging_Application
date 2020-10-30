#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FileIO3.h"

/* global definition */
	/* max string length	*/
int match = 0;


/* function definitions */
void PrintLoginMenu()
{
	printf("\n------------------------------------------\n");
	printf("Welcome! Please choose an option.\n");
	printf("1: Create an Account\n");
	printf("2: Log In\n");
	printf("0: Exit\n");
	printf("Please choose an option: ");
}

/*char* ClientRegister(const char cUser[SLEN], const char cPass[SLEN], char *PassRegisterProtocol)
{
		char RegisterProtocol[69]; 
		strcpy(RegisterProtocol, "REGIS: Username: ");
		strcat(RegisterProtocol, cUser);
		strcat(RegisterProtocol, "  Password: ");
		strcat(RegisterProtocol, cPass);	 
		printf("RegProtocol being sent is: %s: \n", RegisterProtocol);
		strcpy(RegisterProtocol, PassRegisterProtocol);
       	return PassRegisterProtocol;
       	
}*/

int ServerRegister(const char cUser[SLEN], const char cPass[SLEN])
{
    FILE *File;
 	char userMatch[SLEN];
    char passMatch[SLEN];
    
    File = fopen("RegisteredUsers.txt", "r+");
    if(File)
    {
		while(!feof(File))
		{
            fscanf(File, "Username: %19s \t\tPassword: %19s\n", userMatch, passMatch);       
	    	if(strcmp(cUser, userMatch) == 0)
	   	 	{
				return 1;
            }
    	} /* elihw */
    	fclose(File);
	}
	
	File = fopen("RegisteredUsers.txt", "a");
	
	if (!File)
   	{
		printf("Error with Account Registration. Please try again later.\n");
		return 0;
    }
    
    fprintf(File, "Username: %19s \t\tPassword: %19s\n", cUser, cPass);
    fclose(File);
    return 2;
}

/*char* ClientLogIn(const char cUser[SLEN], const char cPass[SLEN], char *PassLoginProtocol)
{
	char LoginProtocol[46];
	strcpy(LoginProtocol, "LOGIN");
	strcat(LoginProtocol, cUser);
	strcat(LoginProtocol, " ");
	strcat(LoginProtocol, cPass);
	printf("LoginProtocol generated in func: %s: ", LoginProtocol);
	strcpy(PassLoginProtocol, LoginProtocol);
	return PassLoginProtocol; 
}*/

int ServerLogIn(const char cUser[SLEN], const char cPass[SLEN])
{
    FILE *File;

    match = 2;
    char userMatch[SLEN];
    char passMatch[SLEN];

    File = fopen("RegisteredUsers.txt", "r+");
 
    if (File == NULL)
    {
		printf("Error logging in. Come back later.");
 		return 0;
    }
    
    else
    {
		while(!feof(File))
		{
            fscanf(File, "Username: %19s \t\tPassword: %19s\n", userMatch, passMatch);       
	    	if(strcmp(cUser, userMatch) == 0 && strcmp(cPass, passMatch) == 0)
	   	 	{
				match = 1;
				break;
            }
    	} /* elihw */
    }

    fclose(File);
	
    if (match == 1)
    {
		printf("Login successful.\n");
		return 1;
    }   
    else
    {
		printf("Invalid username or password.\n");	
    }

    return 0;

}

char* FL_FileLoad(USER *u)
{
	return "c";
}

void FL_FileSave(USER *u, const char *s)
{

}



int getch() 
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/*function for password encrypt*/
void encrypt(char password[], int key)
{
    unsigned int i;
    for(i = 0; i < strlen(password); ++i)
    {
        password[i] = password[i] - key;
    }
}

/*function for password decrypt*/
void decrypt(char password[], int key)
{
    unsigned int i;
    for(i = 0; i<strlen(password); ++i)
    {
        password[i] = password[i] + key;
    }
}
