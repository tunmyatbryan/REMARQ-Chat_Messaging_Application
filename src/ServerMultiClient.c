/*###############################################*/
/*#	ServerMultiClient.c								#*/
/*#		Server for REMARQ chat client				#*/
/*#		Implemented using Non-Blocking			#*/
/*#		reads and writes, designed to handle	#*/
/*#		multiple clients/convorsations at		#*/
/*#		once												#*/
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
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>

#include "DataStructures.h"
#include "Functions.h"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[])
{
	/*Variable Declarations*/
	int serversock, portno, i, n, m, q, p;
	int TempID, newsock;
	fd_set ActiveFDSet, ReadFDSet;
	char LoginString[256];
	char CommBuffer[256];
	char TempBuffer[256];
	char DesiredMessage[256];
	char TempUserName[20];
	int LoginUpdate = 0; 	/*Flag for when new user  logs in*/
	int RegisterUpdate = 0;	/*Flag for when new user registers*/							
	
	/*MESSAGE *NewMessage = NULL;*/
	USER *DesiredUser = NULL;
	UENTRY *DesiredEntry = NULL;
	
	
	
	socklen_t clilen;

	int OnlineArray[50];

	struct pollfd fd;	

	struct sockaddr_in serv_addr, cli_addr;
	
	/*Socket initialization*/
	if (argc < 2) 
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	serversock = socket(AF_INET, SOCK_STREAM, 0);
	if (serversock < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(serversock, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(serversock,5);
	clilen = sizeof(cli_addr);
		
		
	/*Initialize Set of Active Sockets*/
	FD_ZERO(&ActiveFDSet);
	FD_SET(serversock, &ActiveFDSet);

	/*Initialize User List*/
	ULIST *OnlineUsers = CreateUserList();

	/*Initialize Online Array*/
	for(i=0; i<50; i++)
	{
		OnlineArray[i] = 0;	
	}

	/*Initialize Message Queue*/
	MLIST *Queue = CreateMessageList();

	/*Main Server Loop*/
	while(1)
	{
		ReadFDSet = ActiveFDSet;
		/*Blocks until one or more of the active sockets has a request to write to it*/
		/*Stores those sockets in ReadFDSet*/
		printf("This prints before SELECT...\n");
		if(select(FD_SETSIZE, &ReadFDSet, NULL, NULL, NULL) < 0)
		{
			error("ERROR on Select");
		}
		printf("made it past SELECT...\n");
		for(i=0; i<FD_SETSIZE; ++i)
		/*Loop Iterates through all sockets that clients are attmpting to write to and handles the requests*/
		/*READ LOOP*/
		{
			if(FD_ISSET(i, &ReadFDSet))
			{
				if(i == serversock)
				/*Check for connection request on original socket*/				
				{
					p = 0;
					newsock = accept(serversock, (struct sockaddr *) &cli_addr, &clilen);
					if (newsock < 0)
						error("ERROR on accept");
					fd.fd = newsock;
					fd.events = POLLIN;
					n = poll(&fd, 1, 30000);
					switch(n) 
					{
						case -1:
							error("ERROR on poll");
							break;
						case 0:
							printf("Timout waiting for user to send credentials\n");
							m = 0;
							break;
						default:
							bzero(LoginString, 255);
							m = read(newsock,LoginString,255);
							printf("DEBUG Login token from client: %s \n", LoginString);
							break;
					}
					if(m>0)
					{
						/*See if it is a register or login request*/
						n = protocolHandler(LoginString);
						printf("protocolHandler returned %d \n", n);
						if(n>0)
						/*LOGIN REQUEST*/
						{
							q = ServerLogIn(LoginString);
							printf("ServerLogin has returned %d \n", q);
							switch(q)
							{
								case -2:
								/*Invalid Username*/	
									p = 0;
									bzero(CommBuffer, 255);
									strcpy(CommBuffer,"ERROR Invalid Username");
									if(write(newsock, CommBuffer, strlen(CommBuffer)) < 0)
										error("ERROR on Invalid Username write");		
									close(newsock);
									break;
								case 0:
								/*Invalid Password*/
									p = 0;
									bzero(CommBuffer, 255);
									strcpy(CommBuffer,"ERROR Invalid Password");
									if(write(newsock, CommBuffer, strlen(CommBuffer)) < 0)
										error("ERROR on Invalid Password write");		
									close(newsock);
									break;
								case -1:
								/*Error*/
									error("ERROR in TestLogin");
									break;
								case 1:
								/*Valid Login*/
									p = 1;
									LoginUpdate = 1;
									bzero(CommBuffer, 255);
									strcpy(CommBuffer,"SUCCESS on new user login");
									if(write(newsock, CommBuffer, strlen(CommBuffer)) < 0)
										error("ERROR on success login write");		
									break;
								default:
									error("ERROR unexpected response from TestLogin");
									break;
							}
						}
						else if(n == 0)
						/*REGISTER REQUEST*/
						{
							printf("Made it to register request case\n");
							q = ServerRegister(LoginString);
							printf("Server Register Returned %d \n", q);
							switch(q)
							{
								case -1:
								/*Invalid input*/
									p = 0;
									bzero(CommBuffer, 255);
									strcpy(CommBuffer, "ERROR Invalid Input");
									if(write(newsock, CommBuffer, strlen(CommBuffer)) < 0)
										error("ERROR on Invalid Input write");		
									close(newsock);
									break;
								case 0:
								/*Error*/
									error("ERROR in RegisterUser");
									break;
								case 1:
								/*Successful Registration*/
									if(UserDataUpdate(LoginString) < 0)
										error("ERROR on updating user data");
									p = 1;
									RegisterUpdate = 1;
									LoginUpdate = 1;
									bzero(CommBuffer, 255);
									strcpy(CommBuffer,"SUCCESS on new user registration");
									if(write(newsock, CommBuffer, strlen(CommBuffer)) < 0)
										error("ERROR on success registration write");		
									break;
								default:
									error("ERROR Unexpected Response from server register");
							}
						}
						else if(n<0)
							error("ERROR on protocol handler");	
						if(p == 1)
						/*Successful Login or Register*/
						{
							/*Get UserID*/
							TempID = ReturnUserID(LoginString);
							if(TempID<0)
								error("ERROR error on ID lookup");
							printf("DEBUG TempID = %d \n", TempID);
							/*Get Username*/
							char c;
							int x,y;
							x = y = 0;
							if(LoginUpdate == 1 && RegisterUpdate == 0)
							{
								y = 6;
								c = LoginString[6];
							}						
							else if(RegisterUpdate == 1 && LoginUpdate == 1)
							{
								y = 9;
								c = LoginString[9];
							}
							else
								error("ERROR unexpected flags in successful login");
							while((c!=' ')&&(c!='\0'))
							{
								TempUserName[x] = c;
								x++;
								c = LoginString[y + x];						
							}
							printf("DEBUG TempUserName: %s \n", TempUserName);
							/*Add the new user to the user list*/
							USER *NewUser = CreateUser(TempUserName, TempID, newsock);
							AppendUserList(OnlineUsers, NewUser);
							/*Add user to the OnlineArray*/
							for(q=0;q<50;q++)
							{
								if(OnlineArray[q] == 0)
								{
									OnlineArray[q] = TempID;
									break;
								}
							}
							/*Add new connection to fd list*/
							FD_SET (newsock, &ActiveFDSet);

							/*Send all userdata to  the user*/
							bzero(CommBuffer, 255);
							/*j = read(newsock, CommBuffer, 255);*/
							/*SEND AS A FILE*/
							if(send_UserNametxt(newsock) > 0)	
								error("ERROR on sending usernames.txt to new login");		
							/*Make the socket non-blocking*/
							if(fcntl(newsock, F_SETFL, O_NONBLOCK) < 0)
								error("Error on socket non-blocking");
						}
					}
					else if(m == 0)
					/*TIMEOUT*/					
					{
						bzero(CommBuffer, 255);
						strcpy(CommBuffer, "ERROR did not recieve login token");
						if(write(newsock, CommBuffer, strlen(CommBuffer)) < 0)
								error("ERROR on timeout write");
						close(newsock);		
					}
					else if(m<0)
						error("ERROR on read");
				}
				else
				/*Check remaining sockets*/
				{
					/*Check if the user Is sending a message and to whom*/
					bzero(TempBuffer, 255);
					n = read(i, TempBuffer, 255);
					/*Message will have to be packaged as <DestinationID><SenderID><Message>*/
					if(n<0)
						error("ERROR on read");
					else if(n == 0)
					{
						/*CLIENT HAS DISCONNECTED*/
						/*Remove user from online list*/
						UserDisconnect(i, OnlineUsers);
						/*Remove user from online array*/
						for(m=0; m<50; m++)
						{
							if(OnlineArray[m] == i)
								OnlineArray[m] = 0;
						}
						/*Remove I from ActiveFDSet*/
						FD_CLR (i, &ActiveFDSet);
						/*Close i*/
						close(i);
					}
					else if(n>0)
					{
						if(ifQuit(TempBuffer) == 1)
						/*USER WANTS TO DISCONNECT*/
						{
							printf("User wants to disconnect\n");
							/*Remove user from online list*/
							UserDisconnect(i, OnlineUsers);
							/*Remove user from online array*/
							for(m=0; m<50; m++)
							{
								if(OnlineArray[m] == i)
									OnlineArray[m] = 0;
							}
							/*Remove I from ActiveFDSet*/
							FD_CLR (i, &ActiveFDSet);
							/*Close i*/
							close(i);							
						}
						else
						{
							/*Find the FD/user number for desired recipient*/
							int DesiredID;
							int DesiredFD;
	
							DesiredID = ReceiverID(TempBuffer);
							bzero(DesiredMessage, 255);
							strcpy(DesiredMessage, getMessage(TempBuffer));
						
							MESSAGE *NewMessage = NULL;
							USER *DesiredUser = NULL;
							UENTRY *DesiredEntry = OnlineUsers->First;
							for(m = 0; m<OnlineUsers->Length;m++)
							{
								if(DesiredEntry == NULL)
								{
									printf("Desired User is not online\n");
									break;
								}
								DesiredUser = DesiredEntry->Userptr;
								if(DesiredUser->UserID == DesiredID)
								/*Package message with FD in the message buffer*/							
								{
									DesiredFD = DesiredUser->FD;
									NewMessage = CreateMessage(DesiredFD, i, DesiredMessage);
									AppendMessageList(Queue, NewMessage);
									break;
								}
								else
										DesiredEntry = DesiredEntry->Next;
							}
						}
					}
				}
			}		
		}	
		/*End READ LOOP*/
		printf("DEBUG Reached the end of the Read Loop\n");
		printf("DEBUG Beginning Write Loop:\n Rup = %d \n Lup = %d \n", RegisterUpdate, LoginUpdate); 
		for(i=0; i<FD_SETSIZE; ++i)
		/*Iterates through all active sockets and checks if there is pending messages for them*/
		/*Pushes an update if necessary*/
		/*WRITE LOOP*/
		{
			if(FD_ISSET(i,&ActiveFDSet))
			{
				if(i != serversock)
				/*Don't write to the server socket the server socket*/
				{
					/*Check Update Flags*/
					if(LoginUpdate == 1)
					{
						/*Send the online array*/
						bzero(CommBuffer, 255);
						strcpy(CommBuffer,"LUPDATE");
						if(write(i, CommBuffer, strlen(CommBuffer)) < 0)
							error("ERROR on L Update write");
						bzero(CommBuffer, 255);
						n = -1;
						while(n<0)
						{
							n = read(i, CommBuffer, 255);
							if(n == 0)
							{
								/*CLIENT HAS DISCONNECTED*/
								/*Remove user from online list*/
								UserDisconnect(i, OnlineUsers);
								/*Remove user from online array*/
								for(m=0; m<50; m++)
								{
									if(OnlineArray[m] == i)
										OnlineArray[m] = 0;
								}
								/*Remove I from ActiveFDSet*/
								FD_CLR (i, &ActiveFDSet);
								/*Close i*/
								close(i);
							}
						}
						if(write(i, OnlineArray, sizeof(int)*50) < 0)
							error("ERROR on sending online array");
					}
					if(RegisterUpdate == 1)
					{
					/*Send the whole user info file*/
						bzero(CommBuffer, 255);
						strcpy(CommBuffer, "RUPDATE");
						if(write(i, CommBuffer, strlen(CommBuffer)) < 0)
							error("ERROR on R Update write");
						bzero(CommBuffer, 255);
						n = -1;
						while(n<0)
						{
							n = read(i, CommBuffer, 255);
							if(n == 0)
							{
								/*CLIENT HAS DISCONNECTED*/
								/*Remove user from online list*/
								UserDisconnect(i, OnlineUsers);
								/*Remove user from online array*/
								for(m=0; m<50; m++)
								{
									if(OnlineArray[m] == i)
										OnlineArray[m] = 0;
								}
								/*Remove I from ActiveFDSet*/
								FD_CLR (i, &ActiveFDSet);
								/*Close i*/
								close(i);
							}
						}
						if((send_UserNametxt(i)) > 0)	
							error("ERROR on sending usernames.txt to new login");
					}
					
						
					/*Iterate through message queue and see if there is a message waiting for this i*/
					bzero(TempBuffer,255);
					MESSAGE *CurrentMessage = NULL;
					MENTRY *CurrentEntry = Queue->First;
					if(CurrentEntry != NULL)
					{
						for(m=0; m<Queue->Length; m++)
						{
							if (CurrentEntry == NULL)
							{
								printf("Reached the end of the message Queue");
								break; 
							}
							CurrentMessage = CurrentEntry->MsgPtr;
							if(CurrentMessage->FDTo == i)
							{
								int SenderFD = CurrentMessage->FDFrom;
								int SenderID;
								DesiredUser = NULL;
								DesiredEntry = OnlineUsers->First;
								for(q = 0; q < OnlineUsers->Length ; q++)
								{
									if(DesiredEntry == NULL)
									{
										printf("Desired User is not online\n");
										break;
									}
									DesiredUser = DesiredEntry->Userptr;
									if(DesiredUser->FD == SenderFD)
									/*Get ID of sende*/				
									{
										SenderID = DesiredUser->UserID;
										break;
									}
									else
										DesiredEntry = DesiredEntry->Next;
								}
								bzero(TempBuffer,255);
								char SenderIDString[4];
								sprintf(SenderIDString, "%d", SenderID);
								printf("DEBUG itoa: *%s*", SenderIDString);
								strcpy(TempBuffer, SenderIDString);
								strcat(TempBuffer, " ");
								strcat(TempBuffer, CurrentMessage->buffer);
								printf("DEBUG The message string is: %s \n", TempBuffer);
								n = write(i, TempBuffer, 255);
								printf("Sent Message: %s to socket %d\n", TempBuffer, i); 
								if(n<0)
									error("ERROR on write");
								CurrentMessage = DeleteMessageEntry(CurrentEntry);
								DeleteMessage(CurrentMessage);
							}
							else
								CurrentEntry = CurrentEntry->Next;
						}
					}
					else
						printf("MessageQueue is empty\n");
				}
			}	
		}
		/*END WRITE LOOP*/
		LoginUpdate = 0;
		RegisterUpdate = 0;
	}	
	/*End While Loop*/
	return 0;
}
/*#########################################################################*/
