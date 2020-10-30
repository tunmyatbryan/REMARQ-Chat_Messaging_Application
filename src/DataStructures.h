#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#define SLEN 20

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct User USER;
typedef struct UserListEntry UENTRY;
typedef struct UserList	ULIST;

struct User
{
	char Username[SLEN];
	int UserID;
	int FD;
};

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

ULIST* CreateUserList();

void DeleteUserList(ULIST *list);

USER* DeleteUserEntry(UENTRY *e);

void DeleteUser(USER *user);

USER* CreateUser(char Name[SLEN], int ID, int FileDis);

void AppendUserList(ULIST *list, USER *user);


typedef struct Message MESSAGE;
typedef struct MessageListEntry MENTRY;
typedef struct MessageList MLIST;

struct Message
{
	int FDTo;
	int FDFrom;
	char buffer[256];
};

struct MessageListEntry
{
	MLIST *List;
	MENTRY *Next;
	MENTRY *Prev;
	MESSAGE *MsgPtr;
};

struct MessageList
{
	int Length;
	MENTRY *First;
	MENTRY *Last;
};

MLIST* CreateMessageList();

void DeleteMessageList(MLIST *list);

MESSAGE* DeleteMessageEntry(MENTRY *e);

void DeleteMessage(MESSAGE* message);

MESSAGE* CreateMessage(int to, int from, char buff[256]);

void AppendMessageList(MLIST *list, MESSAGE *msg);

#endif 

