/*###############################################*/
/*#	ServerNB.c											#*/
/*#		Server for REMARQ chat client				#*/
/*#		Implemented using Non-Blocking			#*/
/*#		reads and writes								#*/
/*#	Author: 	Joshua Miller							#*/
/*#				jemille2@uci.edu						#*/
/*#	Date:	3/3/2018										#*/
/*###############################################*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

/*Main function*/

int main(int argc, char *argv[])
{
	/*Variable Declarations*/
	int sockfd, portno, pid;
	int newsockfd;
	int fd[2];
	int sockarray[2];
	int clientnum = -1;
	int i,j,n,clientreturn;
	int cmp;	
	char buffer[256];
	int esc = 0;

	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	/*Array initialization*/
	for(i=0; i<2; i++)
	{
		sockarray[i] = 0;			
	}
	/*Socket initialization*/
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
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	
	/*Wait for two clients to connect*/
	j = 0;
	while(j<2)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd <0)
			error("ERROR on accept");
		if(fcntl(newsockfd, F_SETFL, O_NONBLOCK) < 0)
			error("Error on socket non-blocking");

		/*add new socket to array*/
		clientnum = -1;
		   
		for(i=0;i<2;i++)
		{
			cmp = sockarray[i];
			if(cmp == 0)
			{
				printf("Client %d Open\n", i);
				clientnum = i;
				i = 2;
				j++;
			}
		}
		if(clientnum == -1)
			error("Error socket array is full");
		
		sockarray[clientnum] = newsockfd;
		printf("DEBUG Sockets: %d %d\n", sockarray[0], sockarray[1]); 
	}
	bzero(buffer, 255);
	buffer[0] = '&';
	n = write(sockarray[1], buffer, strlen(buffer));
	if(n<0)
		error("ERROR writing to Client 1");
	n = write(sockarray[0], buffer, strlen(buffer));
	if(n<0)
		error("ERROR writing to Client 0");
	
	
	while(1)
	{
		bzero(buffer,256);
		n = read(sockarray[0], buffer, 255);
		if(n>0)
		{
			if(buffer[0] == '~')
			{
				printf("Client 0 has disconnected\n");
				n = write(sockarray[1], buffer, strlen(buffer));
				if(n<0)
					error("ERROR writing to Client 1");
				break;
			}
			printf("Client 0 says: %s\n", buffer);
			n = write(sockarray[1], buffer, strlen(buffer));
			if(n<0)
				error("ERROR writing to Client 1");
			printf("Sent Client 0's message to Client 1\n");
		}
		if(n == 0) 
		{
			printf("Client 1 has disconnected\n");
			break;
		}
		bzero(buffer,256);
		n = read(sockarray[1], buffer, 255);
		if(n>0)
		{
			if(buffer[0] == '~')
			{
				printf("Client 1 has disconnected\n");
				n = write(sockarray[0], buffer, strlen(buffer));
				if(n<0)
					error("ERROR writing to Client 1");
				break;
			}
			printf("Client 1 says: %s\n", buffer);
			n = write(sockarray[0], buffer, strlen(buffer));
			if(n<0)
				error("ERROR writing to Client 0");
			printf("Sent Client 1's message to Client 0\n");
		}
		if(n == 0) 
		{
			printf("Client 0 has disconnected\n");
			break;
		}

	}
	close(sockarray[0]);
	close(sockarray[1]);
	return 0;
}
