#ifndef CLIENTSTRUCT_H
#define CLIENTSTRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "struct.h"





/*	Parameter *u: userList to load
	Parameter *s: buffer from server containing frienduserames. Formatted: "<f1>\n<f2>\n<f3>\0"
	Instanciates the client's friendslist with user types*/
void FL_Load(USER *u, const char *s);

/*	Parameter *u: user/client we are looking at
	Returns: formatted string of friendsusername
	example return of 3 friends:
		"<f1>\n<f2>\n<f3>\0"
		that is:
		"Apollinax\nBitCon\nCez98\0"
	buffer size is handled in function 
	*/
char* FL_Save(USER *u);

/*deletes a list from memory. function calls
	FL_DE and FL_DF as declared below*/
void FL_Delete(USER *u);

/*deletes a specific entry, returns user of the entry*/
USER* FL_DeleteEntry(UENTRY *e);

/*removes a user from memory
later this function will need to be modded
for a friend, if we allow to see a friend's friendsList
since we need to remove f's list from memory
or we will have free<malloc = lost memory!!1*/
void FL_DeleteFriend(USER *f);

/*Adds A friend to the FL.
	Parameters: USER u to mod it's FL
	c-string *f used to create the Friend
	Note:
		1. This does not verify existance of *f
			that must be handled by server before
		2. this does not update server either
			Protocol must be sent by client
	Variation:
		1. change return type to string by calling FL_Save( )
			of 'updated' FL
			1.1 implement the same for FL_Remove
*/
void FL_Append(USER *u, const char *f);

/*removes a friend with username *f from client/user list
this does not update the server's lists/backlogs
client must sent protocol to server to perm delete friend from list
the same way FL_Append works*/
/*might change to return type string 
to return new FL. same for FL_Append*/
void FL_Remove(USER *u, char f[20]);

/*Calls FL_Save, and prints it.*/
void FL_consolePrint(User *u);

#endif
