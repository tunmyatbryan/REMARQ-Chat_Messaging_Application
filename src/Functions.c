/*
	Functions.c
	File to write random functions in.
	Log
	Reza - 3/9 created protocol handler.
			- SERVELOGIN
			- SERVERREGISTER
*/

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
#include "Functions.h"
/*#include "FileIO3.h"*/


/*
	* Protocol Handler
	* Takes in the initial buffer of a new client
	* Returns 0 if Register
	* Returns 1 if Login
	

*/
int protocolHandler(const char buffer[256])
{
		int protocol = -1;
		printf("DEBUG: The buffer is: *%s* \n", buffer);
		
		if (strncmp(buffer, "REGISTER", strlen("REGISTER")) == 0)
			protocol = 0;
		else if (strncmp(buffer, "LOGIN", strlen("LOGIN")) == 0)
			protocol = 1;
			
		if (protocol == -1)
			printf("Bad Protocol. Investigate the initial buffer\n");
		
		return protocol;
}

/*##################################################################*/

int ifQuit(const char tempBuffer[256])
{
	int quit = -1;
	printf("DEBUG: ifQuit function input: *%s*", tempBuffer);
	
	if(strncmp(tempBuffer, "QUIT", strlen("QUIT")) == 0)
		quit = 1;
	else quit = 0;
	
	return quit;
}

/*##################################################################*/

int ServerRegister(const char buffer[256])
{
	/*
	buffer format:
	REGISTER <username> <password>
	register is 8 long, plus space is 9 long 
	*/
	printf("DEBUG ServerRegister buffer: *%s* \n", buffer);
	char cUser[20], cPass[20], userMatch[20], passMatch[20];
	int idMatch;
	bzero(cUser, 20);
	bzero(cPass, 20);
    FILE *File;
    int count = 1;
    char c = buffer[9];
	 char junk[256];
    int i = 0;
    int j = 0;
    
    /*fill in cUser*/
	printf("made it to fill in cUser\n");

    while(c != ' ' && c != '\0')
    {
    	cUser[j] = c;
    	i++; 
    	j++;
    	c = buffer[9 + i];
    }
	printf("DEBUG cUser is %s \n", cUser);    

    /*increment for space btw user pass*/
    i++;
    j = 0;
    c = buffer[9+i];

    printf("DEBUG made it to fill in cPass\n");
    while(c != '\0')
    {
    	cPass[j] = c;
    	i++; 
    	j++;
    	c = buffer[9 + i];
    	/*we never reset */
    }
	printf("DEBUG cPass is %s \n", cPass);

 	File = fopen("RegisteredUsers.txt", "r+");
 	
 	/*If no registered users*/
 	if(!File)
 	{
		printf("DEBUG !File case \n");
 		File = fopen("RegisteredUsers.txt", "a");
 		if (!File)
 		{	
			printf("Error with Registration. Case no Users, making new.\n");
			return 0;
 		}
 		fprintf(File, "Username: %19s \t\tPassword: %19s \t\tID: %4d\n", cUser, cPass, count);
    	fclose(File);
 		return 1;	
 	}
 	
    else if(File)
    {
		printf("DEBUG File case\n");
 
		do
		{
			printf("DEBUG executing compare loop\n");
        	fscanf(File, "Username: %19s \t\tPassword: %19s \t\tID: %d", userMatch, passMatch, &idMatch);  
         	count++;   
	    	if(strcmp(cUser, userMatch) == 0)
	   	 	{
	   	 		printf("Bad Registration. Username in use\n");
				return -1;
            }
    	} while(fgets(junk,sizeof(junk),File) != NULL);
		printf("DEBUG Made it past compare loop \n");
    	fclose(File);
	}
	
	File = fopen("RegisteredUsers.txt", "a");
	if(!File)
	{
		printf("Error opening file, case append user\n");
		return 0;
	}
	
    fprintf(File, "Username: %19s \t\tPassword: %19s \t\tID: %4d\n", cUser, cPass, count);
    fclose(File);
    return 1;
}

/*##################################################################*/

int ServerLogIn(const char buffer[256])
{
	/*
	buffer format:
	LOGIN <username> <password>
	register is 5 long, plus space is 6 long 
	*/
	char cUser[20], cPass[20];
	bzero(cUser, 20);
	bzero(cPass, 20);
    FILE *File;
	
    char c = buffer[6];
    int i = 0, j = 0;
	char junk[256];

	printf("DEBUG SeverLogIn recieved string: %s \n", buffer);
    
    /*fill in cUser*/
    while(c != ' ' && c != '\0')
    {
    	cUser[j] = c;
    	i++; j++;
    	c = buffer[6 + i];
    }
	printf("DEBUG cUser is: %s \n", cUser);    

    /*increment for space btw user pass*/
    i++;
    j = 0;
	 c = buffer[6+i];
    
    while(c != '\0')
    {
    	cPass[j] = c;
    	i++; j++;
    	c = buffer[6 + i];
    	/*we never reset */
    }
 	printf("DEBUG cPass is: %s \n", cPass);    

    int match = 2;
    char userMatch[20];
    char passMatch[20];

    File = fopen("RegisteredUsers.txt", "r");
 
    if (File == NULL)
    {
		printf("Error logging in. Come back later. \n");
 		return -1;
    }
    else
    {
		do
		{
            fscanf(File, "Username: %19s \t\tPassword: %19s", userMatch, passMatch);
				printf("DEBUG userMatch this time is: %s \n", userMatch); 
				printf("DEBUG passMatch this time is: %s \n", passMatch);       
	    	if(strcmp(cUser, userMatch) == 0 && strcmp(cPass, passMatch) == 0)
	   	 	{
				match = 1;
				printf("DEBUG Username/login Match found \n");
				break;
            }
			printf("DEBUG no match found this time \n");
    	} while(fgets(junk,sizeof(junk),File) != NULL);
    }

    fclose(File);
	
    if (match == 1)
    {
		printf("Login successful. \n");
		return 1;
    }   
    else
    {
		printf("Invalid username or password. \n");
		return 0;	
    }
}	

/*##################################################################*/

int ReturnUserID(const char buffer[255])
{
	/*
	buffer format:
	LOGIN <username> <password>
	register is 5 long, plus space is 6 long 
	*/
	int ID;
	char cUser[20], cPass[20];
	bzero(cUser, 20);
	bzero(cPass, 20);
    FILE *File;
    char c = '0';
	char junk[256];
	int i = 0, j = 0;

	printf("DEBUG String passed into ReturnUserID: %s \n", buffer)	;

	if (strncmp(buffer, "REGISTER", strlen("REGISTER")) == 0)
		i = 9;
	else if(strncmp(buffer, "LOGIN", strlen("LOGIN")) == 0)
    	i = 6;
    else
    {
    	printf("Bad Buffer Protocol in ReturnUserID. Returning -1\n");
    	return -1;
    }
    
    
    /*fill in cUser*/
    while(buffer[i] != ' ' && buffer[i] != '\0')
    {
    	cUser[j] = buffer[i];
    	i++; j++;
    }
	printf("DEBUG cUser is: %s \n", cUser);
    
    /*increment for space btw user pass*/
    i++;
    j = 0;
    c = buffer[i];
    
    while(c != '\0')
    {
    	cPass[j] = c;
    	i++;
    	c = buffer[9 + i];
    	/*we never reset */
    }
 
		printf("DEBUG cPass is: %s \n", cPass);

    int match = 2;
    char userMatch[20];
    char passMatch[20];

    File = fopen("RegisteredUsers.txt", "r+");
 
    if (File == NULL)
    {
		printf("Error logging in. Come back later.");
 		return -1;
    }
    
    else
    {
		do
		{
         fscanf(File, "Username: %19s \t\tPassword: %19s \t\tID: %4d", userMatch, passMatch, &ID);
			printf("DEBUG userMatch on this run: %s \n", userMatch);       
	    	if(strcmp(cUser, userMatch) == 0)
	   	 	{
				match = 1;
				printf("DEBUG ID Lookup found match\n");
				break;
            }
			printf("DEBUG no match on this run\n");
    	} while(fgets(junk,sizeof(junk),File) != NULL); /* elihw */
    }

    fclose(File);
	
    if (match == 1)
    {
		printf("ID Lookup Successful\n");
		return ID;
    }   
    else
    {
		printf("ID lookup failed\n");	
    }

    return -1;
}

/*##################################################################*/

/*Pass in a login string, adds credentials to file that stores only Usernames and user ID*/
int UserDataUpdate(const char buffer[256])
{
		/*
	buffer format:
	REGISTER <username> <password>
	-or-
	LOGIN <username> <password>
	register is 8 long, plus space is 9 long 
	*/
	char cUser[20], userMatch[20];
	bzero(cUser, 20);
    FILE *File;
    int count = 1, i = 0, j = 0, dummycount;
    int start;
	 char junk[256];
    
    printf("DEBUG: Buffer into UserDataUpdate() is: *%s*\n", buffer);
    
    if (strncmp(buffer, "REGISTER", strlen("REGISTER")) == 0)
		start = 9;
	else if(strncmp(buffer, "LOGIN", strlen("LOGIN")) == 0)
    	start = 6;
    else
    {
    	printf("Bad Buffer Protocol in ReturnUserID. Returning -1\n");
    	return -1;
    }
    /*set our fill character to the first char of the username*/
    char c = buffer[start];
    
    /*fill in cUser*/
    while(c != ' ' && c != '\0')
    {
    	cUser[j] = c;
    	i++; 
    	j++;
    	c = buffer[start + i];
    }
    
 	File = fopen("Usernames.txt", "r");
 	
 	/*If no Usernames.txt, then first case, count =1;*/
 	if(!File)
 	{
 		count = 1;
 		File = fopen("Usernames.txt", "a");
 		if (!File)
 		{	
			printf("Error with Usernames.txt. Case no Users, making new.\n");
			return 0;
 		}
 		fprintf(File, "Username: %19s \t\tID: %4d\n", cUser, count);
    	fclose(File);
 		return 1;	
 	}
 	 /*there exists users*/
    else if(File)
    {
 
		while(fgets(junk,sizeof(junk),File) != NULL)
		{ 	/*increment count for each user found*/
            fscanf(File, "Username: %19s \t\tID: %4d", userMatch, &dummycount);  
            count++; 
	    	if(strcmp(cUser, userMatch) == 0)
	   	 	{
	   	 		printf("Bad Usernames.txt write in. Username in use\n");
				return -1;
            }
    	}  /* elihw */
    	fclose(File);
	}
	
	File = fopen("Usernames.txt", "a");
	if(!File)
	{
		printf("Error opening file, case append user\n");
		return 0;
	}
	
    fprintf(File, "Username: %19s \t\tID: %4d\n", cUser, count);
    fclose(File);
    return 1;
}

/*##################################################################*/

/*takes in a message protocol buffer, returns the sender ID*/
int ReceiverID(const char buffer[255])
{
	/*buffer format
	<receiverID> "message"*/
	char RecIDStr[4];
	int RecIDInt;
	for(int i = 0; buffer[i] != ' ' || i < 4; i++)
	{
		RecIDStr[i] = buffer[i];
	} 	
	
	RecIDInt = atoi(RecIDStr);
	
	printf("DEBUG in ReceiverID. RecIDStr: %s, atoi: %4d\n", RecIDStr, RecIDInt);
	
	return RecIDInt;
}

/*##################################################################*/

char* getMessage(char buffer[255])
{
	/*
	buffer protocol is:
	<recID> "message"
	*/
	char* FirstChar;
	int i;
	
	for(i= 0; buffer[i] != ' '; i++)
	{
		FirstChar = &buffer[i];
	}
	
	FirstChar = &buffer[i+1];
	return FirstChar;
	
}

/*##################################################################*/

int send_UserNametxt(int socket)
{
	
	FILE *file;
	int size = 0, read_size = 0, size_bytes = 0, package_number = 1;
	
	/*search the file name with "USERNAME.txt" */	
	file = fopen("Usernames.txt", "r");

	if( !file )
	{
		printf("Error: file could not be opened.\n");
		return 1;
	}
			

	printf("Getting file Size\n");   

	/* Getting the size of the file. fseek() moving the pointer to end */
	fseek(file, 0, SEEK_END);
	/* ftell() return the size of the file */
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("Total file size: %d\n",size);

	/* create an array to store the file that is going to send.
	using malloc() to handle huge size file.*/
	char* FileArray = malloc(size);
	if(!FileArray) { 
		printf("Error creating the array to store data.\n");
		return 1;
	}

	char* read_buffer = malloc(size);
	if(!read_buffer) { 
		printf("Error creating the read_buffer array.\n");
		return 1;
	}

	/*Sending file Size to the receiver socket*/
	printf("Sending file Size to receiver.\n");
	write(socket, (void *)&size, sizeof(int));
	
	/*Send file as Array*/
	printf("Sending file as an array\n");

	/*
	Find the size of the file. Returns the number of bytes that were read. 
	If value is negative, then the system call returned an error.
	To make sure that the receiver is ready to receive. To avoid signal errors while reading.
	*/
	do 
	{
		size_bytes=read(socket, read_buffer , size);
	} while (size_bytes < 0);
	printf("Bytes read: %d\n",size_bytes);
	printf("Receiver socket is ready to receive data\n");

	
/* feof(file) function returns a non-zero value when End-of-File indicator associated with the stream is set, else zero is returned.*/
	while(!feof(file)) 
	{
		/*Read from the file into the array that is going to be sent*/
		read_size = fread(FileArray, 1, size, file);
		
		/*Send data through the socket */
		do
		{
			size_bytes = write(socket, FileArray, read_size);  
		}while (size_bytes < 0);
		
		printf("Package Number: %d\n",package_number);
		printf("Package Size Sent: %d\n\n",read_size);    		
		
		/*Increment the total number of packages sent*/
		package_number++;
		
		/*After the sending operation, copy zero value into the array */
		bzero(FileArray, sizeof(FileArray));
	}

/* Close the file and free the meory space for malloc()*/
	fclose(file);
	free(read_buffer);
	free(FileArray);
	printf("Usernames.txt Sent successfully!\n");
	return 0;
}

/*##################################################################*/

int GetUserNametxt(int socket)
{ 
	FILE *file;
	int receive_size = 0, size = 0, read_size = 0, write_size = 0, package_number = 1, size_bytes = 0;	
	
	/* Find the size of the file. Returns the number of bytes that were read. 
	If value is negative, then the system call returned an error.*/
	do
	{
		size_bytes = read(socket, &size, sizeof(int));
	}while(size_bytes < 0);
	
	/* create an array to store the file that is going to receive.
	using malloc() to handle huge size file */
	char* FileArray = malloc(size);
	if(!FileArray) { 
		printf("Error creating the array to store data.\n");
		return 1;
	}

	printf("Package received.\n");
	printf("Package size in bytes(Received): %d\n",size_bytes);
	printf("Total file size(Received): %d\n\n",size);
	
	/*Send back success received signal*/
	do
	{
		size_bytes = write(socket, "received the package", sizeof(int));
	}while(size_bytes < 0);
	
	printf("Package successfully received signal sent\n");
	printf(" \n");
	
	/*
	After the package is received, search the file name with "USERNAME_CLIENT.txt".
	*/
	file = fopen("USERNAME_Client.txt", "w");

	if(!file)
	{
		printf("Error: file could not be opened\n");
		return 1;
	}

	/* continue writing before we finish the entire file. read_size = write_size */
	while(receive_size < size){

		do
		{
			read_size = read(socket, FileArray, size);
		}while(read_size <0);

		/* package_number is the number of packages received */	
		printf("Package number received: %d\n", package_number);
		printf("Package size in bytes(Written): %d\n", size_bytes);
		
		/* Writing the reading data into new file */
		write_size = fwrite(FileArray, 1, read_size, file);
		printf("Written file size: %d\n", write_size); 
	
		/* If the read size and write size are different, the writing process is failed */
		if(read_size != write_size) 
		{
			printf("error in reading and writing the file\n");   
			return 1; 
		}
		
		/*Increment the total number of bytes received*/
		receive_size = receive_size + read_size;
		package_number++;
		printf("Total received file size: %d\n\n", receive_size);
	}
	
	/* Close the file and free the meory space for malloc()*/	
	fclose(file);
	free(FileArray);
	printf("USERNAME.txt Received and written into USERNAME_CLIENT.txt successfully!\n");
	return 0;
}

/*##################################################################*/

ULIST* LoadListFromFile()
{
	int count, i, ID;
	char ccount[4];
	char username[20];
	FILE* File;
	USER* SomeUser;
	ULIST* AllUsers = CreateUserList();
	
	File = fopen("Usernames.txt", "r");

	if(!File )
	{
		printf("Error: file could not be opened.\n");
		return NULL;
	}
	else
	{
		fscanf(File, "Count: %4s\n", ccount);
	}
	
	printf("DEBUG: Count from fscanf is *%4s*\n", ccount);
	count = atoi(ccount);
	
	for(i = 0; i < count; i++)
	{
		fscanf(File, "Username: %19s \t\tID: %4d\n", username, &ID);
		SomeUser = CreateUser(username, ID, 0);
		AppendUserList(AllUsers, SomeUser);
	}
	
	return AllUsers;
}

/*##################################################################*/

int UserDisconnect(int i, ULIST *List)
{
	/*Remove user from online list*/
	USER *DeadUser = NULL;
	UENTRY *DeadEntry = List->First;
	int m;
	int r = 0;
	
	for(m = 0; m < List->Length; m++)
	{
		if(DeadEntry == NULL)
		{
			printf("Disconnected User already removed from Online Users List\n");
			r = -1;
			break;
		}
		DeadUser = DeadEntry->Userptr;
		if(DeadUser->FD == i)
		{
			DeadUser = DeleteUserEntry(DeadEntry);
			DeleteUser(DeadUser);
			DeadUser = NULL;
			DeadEntry = NULL;
			r = 1;
			break;
		}	
		else
			DeadEntry = DeadEntry->Next;
	}
	return r;	
}

