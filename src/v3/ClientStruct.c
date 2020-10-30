#include "ClientStruct.h"

void FL_Load(USER *u, const char *s)
{
	assert(u);
	assert(s);
	
	char FriendUsername[20];
	USER* FriendStruct;
	int i;
	
	while(s) /*stops at delimiter \0*/
	{
	
		bzero(FriendUsername, 20);
		for (i = 0; s != '\n' || s!= '\0'; i++)
		{
			FriendUsername[i] = s;
			s++;
		}
		
		FriendStruct = CreateUser(FriendUsername[20]);
		FL_Append(u, FriendStruct);
		s++;
	}
}


char* FL_Save(USER *u)
{
	assert(u);
	assert(u->Friends);
	if(u->Friends->Length = 0)
		return NULL;
	
	UENTRY *e = u->Friends->First;
	char FriendUsername[20];
	int friendsCount = u->Friends->Length;
	int bufferSize = friendsCount*20;
	char buffer[bufferSize];
	bzero(buffer, bufferSize);
	
	while(e)
	{	
		memset(FriendUsername, '\0', sizeof(FriendUsername));
		strcpy(FriendUsername, e->Userptr->Username);
		strcat(buffer, FriendUsername);
		strcat(buffer, "\n");
		e = e->Next;
	}
	
	return buffer;
}


/*deletes a u->Friends from memory, extra parameter to save*/
void FL_Delete(USER *u)
{
	assert(u->Friends);
	UENTRY *e , *n;
	USER *c;
	e = u->Friends->First;
	
	while(e)
	{
		n = e->Next;
		c = FL_DeleteEntry(e);
		FL_DeleteFriend(c);
		e = n;
	}
	
	free(u->List);
}

USER *FL_DeleteEntry(UENTRY *e);
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


void FL_DeleteFriend(USER *f)
{
	assert(f);
	free(f);
}


void FL_Append(USER *u, const char *f);
{
	/*client must notify server thru protocol
	ex write "FL_append client_username newfriend_username"*/
	assert(u);
	assert(u->Friends);
	assert(f);

	UENTRY *e = NULL;
	e = (UENTRY*)malloc(sizeof(UENTRY));
	assert(e);

	e->List = u->Friends;
	e->Next = NULL;
	e->Prev = NULL;
	e->Userptr = CreateUser(f); /*another malloc*/

	if (u->Friends->Last)
	{		
		e->Next = NULL;
		e->Prev = u->Friends->Last;
		u->Friends->Last->Next = e;
		u->Friends->Last = e;
	}

	else
	{
		e->Next = NULL;
		e->Prev = NULL;
		u->Friends->First = e;
		u->Friends->Last = e;
	}

	u->Friends->Length++;
}


void FL_Remove(USER *u, const char *f)
{
	assert(u);
	assert(u->Friends);
	/*client must notify server thru protocol:
	ex: write "FL_Remove client_username oldfriend_username"*/
	
	/*search Friends for username*/
	UENTRY *e = u->Friends->First;
	while(strcmp(e->Username, *f) != 0)
		e = e->Next;
	
	if(e == NULL)
		printf("User not found in FL\n");
	else
	{
		/*Call deleteEntry at found username*/
		USER oldFriend = FL_DeleteEntry(e);
		FL_DeleteFriend(oldFriend);
	}
}

void FL_consolePrint(User *u)
{
	assert(u);
	assert(u->Friends);
	int bufferSize = 20*u->Friends->Length;
	char buffer[bufferSize];
	bzero(buffer, bufferSize);
	
	buffer = FL_Save(u);
	printf("%s's Friends are: \n%s", u->Username, buffer);
}
