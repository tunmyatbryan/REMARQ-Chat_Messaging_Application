#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>  
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <arpa/inet.h>   
#include <netinet/in.h>

/*
This function will receive the file which is sent throught socket. It will handle all the file types.
It will read and write the file. Return 1 if there is an error. Return 0 if receive successfully.
If there is an existing file witht the same name, the function will overwrite the existing file.
Parameter socket is the socket that where data will be received.
example: receive_file(SocketFD);
*/

int receive_file(int socket)
{
	FILE *file;
	int receive_size = 0, size = 0, read_size = 0, write_size = 0, package_number = 1, size_bytes = 0;
	/* user will type the file name that he wants to save with */
	char file_name[40];
	
	
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
	printf("Total file size(Received): %d\n\n",size);
	
	/*Send back success received signal*/
	do
	{
		size_bytes = write(socket, "received the package", sizeof(int));
	}while(size_bytes < 0);
	
	printf("Package successfully received signal sent\n");
	printf(" \n");
	
	/*
	After the package is received, request the user to type the name to save the file.
	*/
	while(1){
		printf("Please enter the name to save the file including file type (max 40 char): ");
		scanf("%19s", file_name);
		file = fopen(file_name, "w");

		if(!file)
		{
			printf("Error: file could not be opened\n");
		}
		else
		{
			break;		
		}
	}

	/* continue writing before we finish the entire file. read_size = write_size */
	while(receive_size < size){

		do
		{
			read_size = read(socket, FileArray, size);
		}while(read_size <0);

		/* package_number is the number of packages received */	
		printf("Package number received: %d\n", package_number);
		
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
	printf("File Received and written successfully!\n");
	return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void FatalError(const char *Program, const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!", stderr);
    exit(20);
} /* end of FatalError */

int main(int argc , char *argv[])
{

    int SocketFD,	/* socket file descriptor */
	PortNo;		/* port number */
    struct sockaddr_in
	ServerAddress;	/* server address we connect with */
    struct hostent
	*Server;	/* server host */
   
    printf("%s: Starting...\n", argv[0]);
    if (argc < 3)
    {   fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
	exit(10);
    }
#ifdef DEBUG
    printf("%s: Looking up host %s on the network...\n", argv[0], argv[1]);
#endif
    Server = gethostbyname(argv[1]);
    if (Server == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", argv[0], argv[1]);
        exit(10);
    }
    PortNo = atoi(argv[2]);
#ifdef DEBUG
    printf("%s: Using port %d...\n", argv[0], PortNo);
#endif
    if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be greater 2000\n",
		argv[0], PortNo);
        exit(10);
    }
#ifdef DEBUG
    printf("%s: Creating a socket...\n", argv[0]);
#endif
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {   FatalError(argv[0], "socket creation failed");
    }
#ifdef DEBUG
    printf("%s: Preparing the server address...\n", argv[0]);
#endif
    memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    memcpy(&ServerAddress.sin_addr.s_addr,
		Server->h_addr_list[0], Server->h_length);
    printf("%s: Connecting to the server...\n", argv[0]);
    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(ServerAddress)) < 0)
    {   FatalError(argv[0], "connecting to server failed");
    }
	
	receive_file(SocketFD);
	
	close(SocketFD);
	
	return 0;
}
