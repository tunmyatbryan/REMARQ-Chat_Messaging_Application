#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FileIO.h"

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
 
    File = fopen("RegisteredUsers.txt", "a");

    if (!File)
    {
	printf("Error with Account Registration. Please try again later.\n");
	return 1;
    }

    fprintf(File, "Username: %19s \t\tPassword: %19s\n", cUser, cPass);
    fclose(File);
    return 0;
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
		printf("Login successful.");
		return 1;
    }   
    else
    {
		printf("Invalid username or password.");	
    }

    return 0;

}
