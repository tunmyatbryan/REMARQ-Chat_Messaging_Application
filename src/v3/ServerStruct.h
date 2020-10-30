#ifndef SERVERSTRUCT_H
#define SERVERSTRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "struct.h"

/*		--ONLINELIST FUNCTIONS--		*/

/*Appends user to List*/
void OL_Login(ULIST* list, USER *u);

/*Removes user from Online List upon client exit*/
void OL_Logout(USER *u);

/*Deletes the OL upon server shutdown*/
void OL_Delete(ULIST *OnlineList);


#endif
