/*
Header file for struct.c
Change Log:
3/1 7:14 PM - Tun	- implement the data structure according to the SoftwareSpec
*/

#ifndef STRUCT_H
#define STRUCT_H

#define SLEN 20

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

typedef struct User USER;
/*typedef struct UserStatus USTATUS;*/
typedef struct UserListEntry UENTRY;
typedef struct UserList ULIST;
/*typedef struct Personal PERSONAL;*/

struct User
{
	char Username[SLEN];
	ULIST *Friends;
	/*PERSONAL *Profile;*/
};

/*
struct UserStatus
{
	int status; //0 - offline, 1 - online
	char hande[20]; //like a nickname
};
*/

struct UserListEntry
{
	ULIST *List;
	UENTRY *Next;
	UENTRY *Prev;
	USER *Userptr;
};

struct UserList
{
	int Length;
	UENTRY *First;
	UENTRY *Last;
};

/*
struct Personal


	char Handle[SLEN];
	char Name[SLEN+SLEN];
	ULIST *Friends;
};
*/

/*	Malloc for struct LIST, all initializations are NULL/0 */
ULIST* Create();





/* FLIST FUNCTIONS*/
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



/*Online List functions work in progress*/
/*Appends user to List*/
void OL_Login(ULIST* list, USER *u);

/*Removes user from Online List upon client exit*/
void OL_Logout(USER *u);

/*Deletes the OL upon server shutdown*/
void OL_Delete(ULIST *OnlineList);



/*		--USER FUNCTIONS--		*/
/*		Variation in Progress		*/

/*create a User*/
USER *CreateUser(const char *username);

/*delete a User*/
void DeleteUser(USER *OldUser);

#endif
