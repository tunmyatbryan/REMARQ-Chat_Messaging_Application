/*This here is an example pardner*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "FileIO3.h"

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
    char buffer1[256];
    char buffer2[256];
    char success[7];

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
        
 	
		char c;			/*a new char to compare keyboard input*/
		char cUser[SLEN+2];		/* username input	*/
		char cPass[SLEN], TempPass[SLEN];	/* password + buffer	*/
		int  ans;			/* user option 		*/
		int loginSucessful = 0;
		int i;
		
		while(loginSucessful == 0) 
		{
			PrintLoginMenu();
			scanf("%d", &ans);
			
			switch(ans)
			{
			case 1:
				printf("Please enter a desired username (max 20 char.): ");
				scanf ("%19s", cUser);
				
				fgets(buffer1, 256, stdin);
				bzero(buffer1, 256);
				
				while(1)
				{
					printf("Please enter desired password (max 20 char.): ");
					i = 0;
					while(1)
					{
						c = getch();
						if(c >= 33 && c <= 126){
							cPass[i++] = c;
							printf("* \b");	
						}
						else if(c == 8 || c == 127){
							if(i > 0){
								i--;
								printf("\b \b");		
							}
						}
						else{
							cPass[i] = '\0';
							break;			
						}
					} 
				/*	scanf ("%19s", cPass);*/

					printf("\nPlease verify the password: ");
					i = 0;
					while(1)
					{
						c = getch();
						if(c >= 33 && c <= 126){
							TempPass[i++] = c;
							printf("* \b");	
						}
						else if(c == 8 || c == 127){
							if(i > 0){
								i--;
								printf("\b \b");		
							}
						}
						else{
							TempPass[i] = '\0';
							break;			
						}
					} 
					/*scanf ("%19s", TempPass);*/
					i = 0;
					if( strcmp(cPass, TempPass) == 0)
					{
						break;
					}
					else
					{
						printf("\nPassword do not match.\n");
					}
				}	


		/*		scanf ("%19s", cPass);		*/
				encrypt(cPass, 10);

				printf("\n");
        		printf("Sending register protocol to server\n");
        		
        		n = write(sockfd, "REGISTER", strlen("REGISTER"));
        		if (n < 0) error("ERROR writing to socket");
        		bzero(success, 7);
        		n = read(sockfd, success, 7);
				if (n<0) error("ERROR reading from socket");
				if (strcmp(success, "success") != 0)
				{
					printf("Bad communication c1. Exiting");
					close(sockfd);
				}
				
        		n = write(sockfd, cUser, strlen(cUser));
        		if (n < 0) error("ERROR writing to socket");
        		bzero(success, 7);
        		n = read(sockfd, success, 7);
				if (n<0) error("ERROR reading from socket");
				if (strcmp(success, "success") != 0)
				{
					printf("Bad communication c2. Exiting");
					close(sockfd);
				}
				
        		n = write(sockfd, cPass, strlen(cPass));
        		if (n < 0) error("ERROR writing to socket");
           		bzero(success, 7);
        		n = read(sockfd, success, 7);
				if (n<0) error("ERROR reading from socket");
				if (strcmp(success, "success") != 0)
				{
					printf("Bad communication c3. Exiting");
					close(sockfd);
				}
        	 	
				n = write(sockfd, "register data sent", strlen("register data sent"));
				if (n < 0) error("ERROR writing to socket");
        	 	
        	 	bzero(buffer1,256);
        	 	/*read from server with response */
        	 	n = read(sockfd, buffer1, 40);
        	 	printf("Server Response: '%s' \n", buffer1);
        	 	if (n < 0) error("ERROR reading to socket");
       
        	 	
				break;
				
			case 2:
				printf("Enter your username: ");
				scanf ("%19s", cUser);		
				
				fgets(buffer1, 256, stdin);
				bzero(buffer1, 256);
				
				printf("Enter your password: ");
				i = 0;
				while(1)
				{
					c = getch();
					if(c >= 33 && c <= 126){
						cPass[i++] = c;
						printf("* \b");	
					}
					else if(c == 8 || c == 127){
						if(i > 0){
							i--;
							printf("\b \b");		
						}
					}
					else{
						cPass[i] = '\0';
						break;			
					}
				} /*scanf ("%19s", cPass);*/

				encrypt(cPass, 10);
				printf("\n");
			
				
				printf("Sending login protocol to server\n");
				n = write(sockfd, "LOGIN", strlen("LOGIN"));
				bzero(success, 7);
        		n = read(sockfd, success, 7);
				if (n<0) error("ERROR reading from socket");
				if (strcmp(success, "success") != 0)
				{
					printf("Bad communication c4. Exiting");
					close(sockfd);
				}
				
        		n = write(sockfd, cUser, strlen(cUser));
        		if (n < 0) error("ERROR writing to socket");
        		bzero(success, 7);
        		n = read(sockfd, success, 7);
				if (n<0) error("ERROR reading from socket");
				if (strcmp(success, "success") != 0)
				{
					printf("Bad communication c5. Exiting");
					close(sockfd);
				}
				
        		n = write(sockfd, cPass, strlen(cPass));
        		if (n < 0) error("ERROR writing to socket");
           		bzero(success, 7);
        		n = read(sockfd, success, 7);
				if (n<0) error("ERROR reading from socket");
				if (strcmp(success, "success") != 0)
				{
					printf("Bad communication c6. Exiting");
					close(sockfd);
				}
        		
        		n = write(sockfd, "login data sent", strlen("login data sent"));
				if (n < 0) error("ERROR writing to socket");
				
        		/*read login response from server*/
        		bzero(buffer1, 256);
				n = read(sockfd, buffer1, 20);
				if (n < 0) error("Error reading to socket");
				printf("Server Response: %s\n", buffer1);	
				
				if (strcmp(buffer1, "Login Success!") == 0)
					loginSucessful = 1;
				else
					printf("Bad Login communication\n");
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
	
	printf("\n\n");
    int esc, emptyString, MsgCount = 0;
  	/*int cmp;*/
    esc = 0;
    /*Loop Start*/
    while(esc == 0)
    {
	    printf("Please enter the message: ");
	    bzero(buffer1,256);
	   /*scanf ("%233s", buffer1);*/
	   
	   emptyString = 1;
	   while(emptyString)
	   {
	   		fgets(buffer1, 233, stdin); 
	   		/*printf("buffer1 length %d", strlen(buffer1));*/
	   		if (strlen(buffer1) > 1) 
			{
	   			emptyString = 0;
				MsgCount = 1;
			}
	   		else
	   		{
				if(MsgCount == 1){
		   			printf("\nEmpty string! ");
		   			printf("Please enter the message: ");
				}
	   		}
	   	} 
	   	  
	    /*Check for escape*/
	    if(buffer1[0] == '~')
	    {
		esc = 1;
		close(sockfd);
	    }
	    
	    /*concatenate the "cUser: " to buffer1*/
	    bzero(buffer2, 256);
	    strcpy(buffer2, cUser);
	    strcat(buffer2, ": ");
	    strcat(buffer2, buffer1);
	    
	    /*msg protocol sending...*/
	    n = write(sockfd, "MESSAGE", strlen("MESSAGE"));
	    if (n < 0) error("ERROR writing to socket");
	    bzero(success, 7);
       	n = read(sockfd, success, 7);
		if (n<0) error("ERROR reading from socket");
		if (strcmp(success, "success") != 0)
		{
				printf("Bad communication c7. Exiting");
				close(sockfd);
		}
		
	    n = write(sockfd,buffer2,strlen(buffer2));
	    if (n < 0) 
        	 error("ERROR writing to socket");
	    bzero(buffer2, 256);
	    bzero(buffer1, 256);
	    
	    n = read(sockfd,buffer1,255);
	    if (n < 0) 
        	 error("ERROR reading from socket");
	    printf("%s\n",buffer1);
    }
    close(sockfd);
    return 0;
}
