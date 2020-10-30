/*###############################################*/
/*#	PipeClientTest.c									#*/
/*#		Client for REMARQ chat program			#*/
/*#		Implemented using parallel processes	#*/
/*#		and pipes										#*/
/*#	Author: 	Joshua Miller							#*/
/*#				jemille2@uci.edu						#*/
/*#	Date:	3/3/2018										#*/
/*###############################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>



void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   /*Declarations*/
   int sockfd, portno, n;
   int esc = 0;
   int cmp, pid, fd[2], fd2[2];
   struct sockaddr_in serv_addr;
   struct hostent *server;
   char buffer[256];
	char stop[4] = "stop";

	/*Establish connection to desired server*/
   if (argc < 3)
   {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
   portno = atoi(argv[2]);
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
       error("ERROR opening socket");
   server = gethostbyname(argv[1]);
   if (server == NULL)
	{ 
       fprintf(stderr,"ERROR, no such host\n");
       exit(0);
   }
   bzero((char *)&serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
   serv_addr.sin_port = htons(portno);
   if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
       error("ERROR connecting");
	if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		error("Error on socket non-blocking");

	/*Spawn pipe to communicate between child and parent process*/
	if(pipe(fd) < 0)
		error("ERROR on pipe 1 creation");
	if (fcntl(fd[0], F_SETFL, O_NONBLOCK) < 0)
				error("ERROR on pipe 1 non-blocking");
	if(pipe(fd2) < 0)
		error("ERROR on pipe 2 creation");
	if (fcntl(fd2[0], F_SETFL, O_NONBLOCK) < 0)
				error("ERROR on pipe 1 non-blocking");

	while(1)
	{
		n = read(sockfd, buffer, 255);
		if(buffer[0] = '&')
			break;
	}
	
   /*Spawn process to handle read and process to handle writes*/
	pid = fork();
	if(pid<0)
		error("ERROR on fork");

	if(pid == 0)
	/*Child process listens*/
	{
		while(esc == 0)
		{
			bzero(buffer,256);
			n = read(sockfd,buffer,255);
			if(n>0)
			{
				if(buffer[0] =='~')
				{
					printf("The other user has disconnected\n");
					close(fd2[0]);
					n = write(fd2[1],stop,strlen(stop));
					esc = 1;
					break;
				}
				printf("\nThe other user says: %s\n",buffer);
			}
			else if(n == 0)
			{
				printf("The other user has disconnected\n");
				close(fd2[0]);
				n = write(fd2[1],stop,strlen(stop));
				esc = 1;
				break;
			}
			close(fd[1]);
			n = read(fd[0],stop,4);
			if(n>=0)
			{
				esc = 1;
			}		
		}
		close(sockfd);
		exit(0);
	}
	else
	/*Parent process writes*/
	{
		while(esc == 0)
   	{
	   	/*printf("Please enter the message: ");*/
			close(fd2[1]);
			n = read(fd2[0],stop,4);
			if(n>=0)
			{
				esc = 1;
				break;
			}
		   bzero(buffer,256);
		   fgets(buffer,255,stdin);
		   /*Check for escape*/
		   if(buffer[0] == '~')
			{
				esc = 1;
				close(fd[0]);
				n = write(fd[1],stop,strlen(stop));
				break;
			}
		   n = write(sockfd,buffer,strlen(buffer));
		   if (n < 0) 
   	   	 error("ERROR writing to socket");
		   /*
			bzero(buffer,256);
		   n = read(sockfd,buffer,255);
		   if (n < 0) 
   	    	 error("ERROR reading from socket");
		   printf("%s\n",buffer);
			*/
   	}
   close(sockfd);
	}
	return 0;
}
/*
void Write()
{
	while(esc == 0)
   {
	   printf("Please enter the message: ");
	   bzero(buffer,256);
	   fgets(buffer,255,stdin);
	   /*Check for escape
	   if(buffer[0] == '~')
	   {
		esc = 1;
	   }
	   n = write(sockfd,buffer,strlen(buffer));
	   if (n < 0) 
       	 error("ERROR writing to socket");
	   bzero(buffer,256);
	   n = read(sockfd,buffer,255);
	   if (n < 0) 
       	 error("ERROR reading from socket");
	   printf("%s\n",buffer);
		    bzero(buffer,256);
	   n = read(sockfd,buffer,255);
	   if (n < 0) 
       	 error("ERROR reading from socket");
	   printf("%s\n",buffer);
   }
   close(sockfd);
}*/
