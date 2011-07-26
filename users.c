#include "users.h"

static v3_user** users;

void users_init( int max_users) {
	int i;
	users = malloc( sizeof( v3_user ) * max_users );
	
	for( i = 0; i < max_users; i++ ) {
		users[i] = NULL;
	}
}

v3_user* users_get( int userid ) {
	int i = 0;
	v3_user* user_ptr = NULL;

	for( i = 0; i < settings.max_users; i++ ) {
		if ( users[i] == NULL ) continue;
		if ( users[i]->id == userid ) user_ptr = users[i];
	}

	return user_ptr;
}

void users_add( int userid ) {
	int i = 0;
	v3_user *user_ptr = v3_get_user( userid );
	
	if ( user_ptr == NULL ) {
		printf( "user is null\n" );
		return;
	}

	while( users[i] != NULL ) i++;

	users[i] = user_ptr;
	printf( "added user: %s\n", user_ptr->name );
}

void users_remove( int userid ) {
	v3_user *user_ptr = users_get( userid );

	if ( user_ptr == NULL ) { 
		printf( "user is null\n" );
		return;
	}

	v3_free_user( user_ptr );
}


