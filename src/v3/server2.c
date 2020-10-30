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

int dostuff(int sock, int socknum, int array[10]); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     int sockarray[10];		/*Array of size 10 to store sockets*/
     int m;
     int debug;
     int i;
	
     for(i=0; i<10; i++)
     /*Initialize the socket array to all 0's*/
     {
	sockarray[i] = 0;
     }

     int socknum = 0; 		/*integer to keep track of number of sockets currently allocated*/

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
	 /*Assign new socket to array*/
	 sockarray[socknum] = newsockfd;
	 socknum++;
	 /*DEBUG print the user array*/
	for(i=0; i<10; i++)
	{
		debug = sockarray[i];
		if(debug != 0)
		{
		   printf("User %d connected on socket %d \n", i, debug);	
		}
		else
		{
		   printf("No socket for user %d \n", i);
		}

	}
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");

         if (pid == 0)  
	 /*create new process for new open socket*/
	 {
             close(sockfd);
             m = dostuff(newsockfd, socknum-1, sockarray);
	     printf("DEBUG m = %d \n", m);
	     if(m != 0)
	     {
		sockarray[m] = 0;
	     }    
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
  There is a separate instance of this function 
  for each connection.  It handles all communication
  once a connnection has been established.
 *****************************************/
int dostuff (int sock, int socknum, int array[10])
{
   int n;
   int tempsock;
   char buffer[256];
   int esc = 0;
	while(esc == 0)
	{   
	   	bzero(buffer,256);
	   	n = read(sock,buffer,255);
	   	if (n < 0)
		{
			error("ERROR reading from socket");
			return 0;
		}
		if(buffer[0] == '~')
		{
			esc = 1;
		}   	
		printf("Here is the message: %s\n",buffer);
	   	n = write(sock,"I got your message",18);
	   	if (n < 0)
		{
			error("ERROR writing to socket");
			return 0;
	   	}
		/*Relay message to other connected clients*/
		for(int i = 0; i<10; i++)
		{
			tempsock = array[i];

			if(tempsock != 0)
			/*write the buffer to that socket*/
			{
				/*n = write(tempsock, "User %d Says: \" %s \" \n", i+1, buffer);*/
				n = write(tempsock, buffer, strlen(buffer));
				if (n < 0)
				{
					error("ERROR writing to socekt");
					return 0;
				}
			}
		}

	}
	return socknum;
}
