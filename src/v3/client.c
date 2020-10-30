/*This here is an example pardner*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    /*Declarations*/
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3) 
    /*Check for proper arguments*/
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    /*Assign variables*/
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*Check that desired socket is valid*/
    if (sockfd < 0) 
        error("ERROR opening socket");

    /*Assign */
    server = gethostbyname(argv[1]);

    if (server == NULL) 
    /*Check that the desired server is valid*/
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    int esc;
    int cmp;
    esc = 0;
    /*Loop Start*/
    while(esc == 0)
    {
	    printf("Please enter the message: ");
	    bzero(buffer,256);
	    fgets(buffer,255,stdin);
	    /*Check for escape*/
	    if(buffer[0] == '~')
	    {
		esc = 1;
	    }
	    n = write(sockfd,buffer,strlen(buffer));
	    if (n < 0) 
        	 error("ERROR writing to socket");
	    bzero(buffer,256);
	    /*Wait for conformation from server*/
	    n = read(sockfd,buffer,255);
	    if (n < 0) 
        	 error("ERROR reading from socket");
	    printf("%s\n",buffer);
	    /*Wait for message from other users*/
	    n = read(sockfd,buffer,255);
	    if (n<0)
		 error("ERROR reading from socket");
	    printf("%s\n",buffer);
    }
    close(sockfd);
    return 0;
}
