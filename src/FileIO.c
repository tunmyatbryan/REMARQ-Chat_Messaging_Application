#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* global definition */
const int SLEN = 20; 		/* max string length	*/
int match = 0;

/* function declarations */
void PrintMenu();
int  Register();
int  LogIn();
/* end of function declarations */

int main()
{

char cUser[SLEN];		/* username input	*/
char cPass[SLEN];		/* password input	*/
int  ans;			/* user option 		*/

    PrintMenu();
    scanf("%d", &ans);
   
    while(ans != 0) 
    {
    switch(ans)
    {
	case 1:
	    printf("Please enter a desired username (max 20 char.): ");
	    scanf ("%19s", cUser);
	    printf("\nPlease enter desired password (max 20 char.): ");
	    scanf ("%19s", cPass);
	    printf("\n\n");
	    
	    Register(cUser, cPass);
	    break;
	case 2:
	    printf("Enter your username: ");
	    scanf ("%19s", cUser);
	    printf("Enter your password: ");
	    scanf ("%19s", cPass);
	    printf("\n\n");

	    LogIn(cUser, cPass);
	    break;
	default:
	    printf("ERROR! Please enter a valid choice.\n"); 
 	    break;
   } /* end switch */
	PrintMenu();
	scanf("%d", &ans);
   } /* end while  */ 
   printf("Exiting the program.\n");
   return 0;
}

/* function definitions */
void PrintMenu()
{
printf("\n------------------REMARQ-------------------\n");
printf("Welcome! Please choose an option.\n"		);
printf("1: Create an Account\n"				);
printf("2: Log In\n"					);
printf("0: Exit\n"					);
printf("Please choose an option.\n\n"			);
}

int Register(const char cUser[SLEN], const char cPass[SLEN])
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

int LogIn(const char cUser[SLEN], const char cPass[SLEN])
{
    FILE *File;

    match = 2;
    char userMatch[SLEN];
    char passMatch[SLEN];

    File = fopen("RegisteredUsers.txt", "r+");
 
    if (File == NULL)
    {
	printf("Error logging in. Come back later.");
 	return 1;
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
    }   
    else
    {
		printf("Invalid username or password.");	
    }

    return 0;
}
