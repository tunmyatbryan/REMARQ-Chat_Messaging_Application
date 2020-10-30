#include <stdlib.h>
#include <assert.h>
#include "struct.h"


ULIST *Create()
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

/*Create a User*/
USER *CreateUser(const char *username)
{
	USER *user = (USER *)malloc(sizeof(USER));
	if (user == NULL)
	{
		return NULL;
	}
	
	memset(user->Username, '\0', sizeof(
	strncpy(user->Username, username, strlen(username));
	user->Username[SLEN] = '\0'
	
	user->Friends = NULL;
	/*can replace by FL_Load(user)*/

	return user;
}

void DeleteUser(USER *OldUser);
{
	/*FL_Delete(OldUser, 0);*/
	assert(OldUser);
	free(OldUser);  	
}

