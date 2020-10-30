/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument 
 *    This version runs forever, forking off a separate 
 *    process for each connection
 *           */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "FileIO3.h"

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 *  There is a separate instance of this function 
 *   for each connection.  It handles all communication
 *    once a connnection has been established.
 *     *****************************************/
void dostuff (int sock)
{
   	int n;
  	int protocol = 0;
   	char buffer[256];
   	char cUser[20];
   	char cPass[20];
   	int esc = 0;
	while(esc == 0)
   	{   
   		bzero(buffer, 256);
  	 	n = read(sock,buffer,255);
  	 	if (n < 0)
		{
			error("ERROR reading from socket");
			return;
		}
		n = write(sock, "success", strlen("success"));
		
		printf("read buffer for protocol is %s \n", buffer);
		/*handle 1st protocol*/
		if (strcmp(buffer, "REGISTER") == 0)
			protocol = 1;
		else if (strcmp(buffer, "LOGIN") == 0)
			protocol = 2;
		else if (strcmp(buffer, "MESSAGE") == 0)
			protocol = 3;	
		bzero(buffer	, 256);
		
		switch(protocol)
		{
			case 1:
				printf("In switch protocol state register\n");
				
				n = read(sock, cUser, 20);
				if (n<0) error("ERROR reading from socket");
				bzero(buffer, 256);
        		n = write(sock, "success", strlen("success"));
				if (n<0) error("ERROR writing to socket");
				
				n = read(sock, cPass, 20);
				if (n<0) error("ERROR reading from socket");
				bzero(buffer, 256);
        		n = write(sock, "success", strlen("success"));
				if (n<0) error("ERROR writing to socket");
				
				bzero(buffer, 256);
				n = read(sock, buffer, 256);
				if (n<0) error("ERROR reading from socket");
				printf("Register msg from client: %s\n", buffer);
				bzero(buffer, 256);
				
				n = ServerRegister(cUser, cPass);
				if (n == 2)
					n = write(sock, "Register Success!", strlen("Register Success!"));
				else if (n == 1)
					n = write(sock, "Register Failed! Duplicate Username", strlen("Register Failed! Duplicate Username"));
				else
					n = write(sock, "Register Failed!", strlen("Register Failed!"));

				if (n<0) error("ERROR writing to socket"); 	
				break;
				
			case 2:
				printf("In switch protocol state login\n");
				
				n = read(sock, cUser, 20);
				if (n<0) error("ERROR reading from socket");
				bzero(buffer, 256);
        		n = write(sock, "success", strlen("success"));
				if (n<0) error("ERROR writing to socket");
				
				n = read(sock, cPass, 20);
				if (n<0) error("ERROR reading from socket");
				bzero(buffer, 256);
        		n = write(sock, "success", strlen("success"));
				if (n<0) error("ERROR writing to socket");
				
				bzero(buffer, 256);
				n = read(sock, buffer, 256);
				if (n<0) error("ERROR reading from socket");
				printf("Login msg from client: %s\n", buffer);
				bzero(buffer, 256);
				
				n = ServerLogIn(cUser, cPass);
				if (n == 1)
					n = write(sock, "Login Success!", strlen("Login Success!"));
				else
					n = write(sock, "Login Failed!", strlen("Login Failed!"));
					
				if (n<0) error("ERROR writing to socket"); 
				break;
				
			case 3:
				printf("In switch protocol state message\n");
				n = read(sock, buffer, 255);
				if(buffer[0] == '~')
				{
					esc = 1;
					return;
				} 
				
				printf("The message is:\n%s\n", buffer);
				n = write(sock,"I got your message",18);
	  	 	
	 	  		if (n < 0)
				{
					error("ERROR writing to socket");
					return;
				}
				
				break;
			default: 
				error("Bad comm");
				esc = 1;
				return;
		} 	
	}
}
