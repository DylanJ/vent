#include "users.h"

void users_init( int max_users) {
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

void users_update( int userid ) {
	for( int i = 0; i < MAX_USERS; i++ ) {
		if ( users[i] == NULL )
			continue;
		if ( users[i]->id == userid ) {
			v3_free_user( users[i] );
			users[i] = v3_get_user( userid );
		}
	}
}

void users_remove( int userid ) {
	v3_user *user_ptr = users_get( userid );

	if ( user_ptr == NULL ) { 
		printf( "user is null\n" );
		return;
	}

	for( int i = 0; i < MAX_USERS; i++ ) {
		if ( users[i] == NULL )
			continue;

		if ( users[i]->id == userid ) {
			users[i] = NULL;
		}
	}

	v3_free_user( user_ptr );
}


