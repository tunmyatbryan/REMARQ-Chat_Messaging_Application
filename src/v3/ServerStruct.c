#include "ServerStruct.h"




/*		--ONLINELIST FUNCTIONS--		*/

/*Appends user to List*/
void OL_Login(ULIST* list, USER *new)
{
	assert(list);

	UENTRY *e = NULL;
	e = (UENTRY*)malloc(sizeof(UENTRY));
	assert(e);

	e->List = list;
	e->Next = NULL;
	e->Prev = NULL;
	e->Userptr = new;

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
/*Removes user from Online List upon client exit*/
void OL_Logout(USER *old)
{
	
}

/*Deletes the OL upon server shutdown*/
void OL_Delete(ULIST *list)
{
	assert(list);
	UENTRY *e , *n;
	USER *c;
	e = list->First;

	while(e)
	{
		n = e->Next;
		c = FL_DeleteEntry(e);
		DeleteUser(c);
		e = n;
	}
	
	free(u->List);
}
