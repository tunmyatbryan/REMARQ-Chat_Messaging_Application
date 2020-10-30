/*This here is an example pardner*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "FileIO.h"



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
        
 	

		char cUser[SLEN];		/* username input	*/
		char cPass[SLEN];		/* password input	*/
		int  ans;			/* user option 		*/
		int loginSucessful = 0;
	   
		while(ans != 0 && loginSucessful == 0) 
		{
			PrintLoginMenu();
			scanf("%d", &ans);
			
			switch(ans)
			{
			case 1:
				printf("Please enter a desired username (max 20 char.): ");
				scanf ("%19s", cUser);
				printf("Please enter desired password (max 20 char.): ");
				scanf ("%19s", cPass);
				printf("\n");
			/*
				char RegisterProtocol[69];
				ClientRegister(cUser, cPass, RegisterProtocol);
				printf("DEBUG RegisterProtocol: %s %c", RegisterProtocol, RegisterProtocol[51]);
				/*RegisterProtocol* = ClientRegister(cUser, cPass);*/
				/*write to server with register data*/
			/*	
				n = write(sockfd,RegisterProtocol, strlen(RegisterProtocol));
	    		if (n < 0) 
        	 		error("ERROR writing to socket");
        	*/ 
        		printf("Sending register protocol to server\n");
        		n = write(sockfd, "REGISTER", strlen("REGISTER"));
        		if (n < 0) error("ERROR reading to socket");
        		n = write(sockfd, cUser, strlen(cUser));
        		if (n < 0) error("ERROR reading to socket");
        		n = write(sockfd, cPass, strlen(cPass));
        		if (n < 0) error("ERROR reading to socket");
        	 		
        	 	bzero(buffer,256);
        	 	/*read from server with response */
        	 	n = read(sockfd, buffer, strlen(buffer));
        	 	printf("Client read 'buffer' after 3x write is: '%s' \n", buffer);
        	 	if (n < 0) error("ERROR reading to socket");
       
        	 	
				break;
				
			case 2:
				printf("Enter your username: ");
				scanf ("%19s", cUser);
				printf("Enter your password: ");
				scanf ("%19s", cPass);
				printf("\n");
			/*
				char LoginProtocol[46];
				ClientLogIn(cUser, cPass, LoginProtocol);
				/*LoginProtocol* = ClientLogIn(cUser, cPass);*/
				/*write to server with login data*/
			/*	n = write(sockfd, LoginProtocol, strlen(LoginProtocol));
				if (n < 0)
					error("ERROR writing to socket");
			*/	
				/*read from server with response
					if response == 1, then loginSucc = 1. */
				
				printf("Sending login protocol to server\n");
				n = write(sockfd, "LOGIN", strlen("LOGIN"));
				if (n < 0) error("ERROR writing to socket");
        		n = write(sockfd, cUser, strlen(cUser));
        		if (n < 0) error("ERROR writing to socket");
        		n = write(sockfd, cPass, strlen(cPass));
        		if (n < 0) error("ERROR writing to socket");
        		
        		bzero(buffer, 256);
				n = read(sockfd, buffer, strlen(buffer));
				if (n < 0) error("Error reading to socket");
				
				printf("output of login buffer: %s", buffer);
				if (strcmp(buffer, "Login Failed!") == 0)
				{
					printf("bad login. readProtocol is %s. Closing client", buffer);
					close(sockfd);
					return 0;
				}	
				else if (strcmp(buffer, "Login Success!") == 0)
					loginSucessful = 1;
				else
					printf("bad login communication\n");
				break;
				
			case 0:
				close(sockfd);
				return 0;
				
				break;
				
			default:
				printf("ERROR! Please enter a valid choice.\n"); 
		 	    break;
			} /* end switch */
		
	   } /* end while  */ 
	   
	
       
        
        
        
        
        /*if we exit the while loop with successful login*/
        

    int esc;
  	/*int cmp;*/
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
		close(sockfd);
	    }
	    
	    n = write(sockfd, "MESSAGE", strlen("MESSAGE"));
	    if (n < 0) 
        	 error("ERROR writing to socket");
	    n = write(sockfd,buffer,strlen(buffer));
	    if (n < 0) 
        	 error("ERROR writing to socket");
	    bzero(buffer,256);
	    
	    n = read(sockfd,buffer,255);
	    if (n < 0) 
        	 error("ERROR reading from socket");
	    printf("%s\n",buffer);
    }
    close(sockfd);
    return 0;
}
