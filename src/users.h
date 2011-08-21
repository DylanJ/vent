#include <stdio.h>
#include <stdlib.h>
#include <ventrilo3.h>
#include "settings.h"

#define MAX_USERS 1024

v3_user* users[MAX_USERS];
void users_init( int max_users );
void users_add( int userid );
void users_remove( int userid );
v3_user* users_get( int userid );
void users_update( int userid );

