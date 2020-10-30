#include "DataStructures.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

ULIST* CreateUserList()
{
	ULIST *newList = (ULIST*)malloc(sizeof(ULIST));

	if (newList != NULL)
	{
		newList->Length = 0;
		newList->First = NULL;
		newList->Last = NULL;
	}

	assert(newList);

	return newList;
}

void DeleteUserList(ULIST *list)
{
	assert(list);
	assert(list->First);
	UENTRY *e , *n;
	USER *c;
	e = list->First;
	
	while(e)
	{
		n = e->Next;
		c = DeleteUserEntry(e);
		DeleteUser(c);
		e = n;
	}
	
	free(list);
}

USER* DeleteUserEntry(UENTRY *e)
{
	assert(e);
	
	if(e->Prev == NULL)
	{
		e->List->First = e->Next;
		if(e->Next)
			e->Next->Prev = NULL;
	
	}
	else if (e->Next == NULL)
	{
		e->List->Last = e->Prev;
		if(e->Prev)
			e->Prev->Next = NULL;
	}
	else 
	{
		e->Prev->Next = e->Next;
		e->Next->Prev = e->Prev;
	}
	
	e->List->Length--;
	USER *u;
	assert(e && e->Userptr);
	u = e->Userptr;
	free(e);
	return(u);
}

void DeleteUser(USER *OldUser)
{
	/*FL_Delete(OldUser, 0);*/
	assert(OldUser);
	free(OldUser);  	
}

USER* CreateUser(char Name[SLEN], int ID, int FileDis)
{
	USER *user = (USER *)malloc(sizeof(USER));
	if (user == NULL)
	{
		return NULL;
	}
	
	memset(user->Username, '\0', 20*sizeof(char));
	strncpy(user->Username, Name, strlen(Name));
	user->FD = FileDis;
	user->UserID = ID;

	return user;
}

void AppendUserList(ULIST *list, USER *user)
{
	assert(list);
	assert(user);

	UENTRY *e = NULL;
	e = (UENTRY*)malloc(sizeof(UENTRY));
	assert(e);

	e->List = list;
	e->Next = NULL;
	e->Prev = NULL;
	e->Userptr = user; /*another malloc*/

	if (list->Last)
	{		
		e->Next = NULL;
		e->Prev = list->Last;
		list->Last->Next = e;
		list->Last = e;
	}

	else
	{
		e->Next = NULL;
		e->Prev = NULL;
		list->First = e;
		list->Last = e;
	}

	list->Length++;
}


MLIST* CreateMessageList()
{
	MLIST *newList = (MLIST*)malloc(sizeof(MLIST));

	if (newList != NULL)
	{
		newList->Length = 0;
		newList->First = NULL;
		newList->Last = NULL;
	}

	assert(newList);

	return newList;
}

void DeleteMessageList(MLIST *list)
{
	assert(list);
	assert(list->First);
	MENTRY *e , *n;
	MESSAGE *c;
	e = list->First;
	
	while(e)
	{
		n = e->Next;
		c = DeleteMessageEntry(e);
		DeleteMessage(c);
		e = n;
	}
	
	free(list);
}


MESSAGE* DeleteMessageEntry(MENTRY *e)
{
	MESSAGE *m;
	assert(e && e->MsgPtr);
	m = e->MsgPtr;
	
	if(e->Prev == NULL)
	{
		e->List->First = e->Next;
		if(e->Next)
			e->Next->Prev = NULL;
	
	}
	else if (e->Next == NULL)
	{
		e->List->Last = e->Prev;
		if(e->Prev)
			e->Prev->Next = NULL;
	}
	else 
	{
		e->Prev->Next = e->Next;
		e->Next->Prev = e->Prev;
	}
	
	e->List->Length--;
	free(e);
	return(m);
}


void DeleteMessage(MESSAGE* message)
{
	assert(message);
	free(message);
}

MESSAGE* CreateMessage(int to, int from, char buff[256])
{
	MESSAGE *message = (MESSAGE *)malloc(sizeof(MESSAGE));
	if (message == NULL)
	{
		return NULL;
	}
	
	memset(message->buffer, '\0', 256*sizeof(char));
	strncpy(message->buffer, buff, strlen(buff));
	message->FDFrom = from;
	message->FDTo = to;

	return message;
}

void AppendMessageList(MLIST *list, MESSAGE *msg)
{
	assert(list);
	assert(msg);

	MENTRY *e = NULL;
	e = (MENTRY*)malloc(sizeof(MENTRY));
	assert(e);

	e->List = list;
	e->Next = NULL;
	e->Prev = NULL;
	e->MsgPtr = msg; /*another malloc*/

	if (list->Last)
	{		
		e->Next = NULL;
		e->Prev = list->Last;
		list->Last->Next = e;
		list->Last = e;
	}

	else
	{
		e->Next = NULL;
		e->Prev = NULL;
		list->First = e;
		list->Last = e;
	}

	list->Length++;
}

