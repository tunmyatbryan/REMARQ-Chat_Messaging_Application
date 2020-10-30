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

/* 
This function will send the USERNAME.txt file throught the socket. It will search the file name "USERNAME.txt".
socket parameter is the socket which will receive the file (it is client socket)
example: send_file(DataSocketFD);
*/
int send_UserNametxt(int socket)
{
	
	FILE *file;
	int size = 0, read_size = 0, size_bytes = 0, package_number = 1;
	
	/*search the file name with "USERNAME.txt" */	
	file = fopen("USERNAME.txt", "r");

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
	printf("USERNAME.txt Sent successfully!\n");
	return 0;
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
    int ServSocketFD,	/* socket file descriptor for service */
	DataSocketFD,	/* socket file descriptor for data */
	PortNo;		/* port number */
    socklen_t ClientLen;
    struct sockaddr_in
	ServerAddress,	/* server address (this host) */
	ClientAddress;	/* client address we connect with */

    printf("%s: Starting...\n", argv[0]);
    if (argc < 2)
    {   fprintf(stderr, "Usage: %s port\n", argv[0]);
	exit(10);
    }
    PortNo = atoi(argv[1]);	/* get the port number */
#ifdef DEBUG
    printf("%s: Using port %d...\n", argv[0], PortNo);
#endif
    if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be greater 2000\n",
		argv[0], PortNo);
        exit(10);
    }
#ifdef DEBUG
    printf("%s: Creating a socket to serve clients...\n", argv[0]);
#endif
    ServSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0)
    {   FatalError(argv[0], "service socket creation failed");
    }
#ifdef DEBUG
    printf("%s: Preparing the server address...\n", argv[0]);
#endif
    memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr.s_addr = INADDR_ANY;
#ifdef DEBUG
    printf("%s: Assigning the server name to the socket...\n", argv[0]);
#endif
    if (bind(ServSocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(ServerAddress)) < 0)
    {   FatalError(argv[0], "binding the server to a socket failed");
    }
    printf("%s: Listening on port %d...\n", argv[0], PortNo);
    if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
    {   FatalError(argv[0], "listening on socket failed");
    }

	ClientLen = sizeof(ClientAddress);
	DataSocketFD = accept(ServSocketFD, (struct sockaddr*)&ClientAddress,
		&ClientLen);
	if (DataSocketFD < 0)
	{   FatalError(argv[0], "data socket creation (accept) failed");
	}
	printf("%s: Accepted connection from client.\n", argv[0]);
#ifdef DEBUG
	printf("%s: Client address:port = %u:%hu.\n", argv[0],
			ClientAddress.sin_addr.s_addr, ntohs(ClientAddress.sin_port));
#endif
	
	/*call the send_file function. DataSocketFD is the socket which wants to send the file*/
	send_UserNametxt(DataSocketFD);
	
	close(ServSocketFD);
	fflush(stdout);
	return 0;
}
