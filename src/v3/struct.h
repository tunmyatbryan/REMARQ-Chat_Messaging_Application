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


/*		--USER FUNCTIONS--		*/
/*		Variation in Progress		*/

/*create a User*/
USER *CreateUser(const char *username);

/*delete a User*/
void DeleteUser(USER *OldUser);

#endif
