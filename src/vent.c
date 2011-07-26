#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <sys/types.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <ventrilo3.h>

#include "vent.h"

void vent_init() {
	settings.max_users = v3_get_max_clients();
	
	users_init( settings.max_users );

	printf( "max clients: %i\n", settings.max_users );
}

void vent_display_motd( int userid, char* comment,
	char* url, char* integration_text ) {
	printf( "MOTD id: %i, %s, %s, %s\n", userid, comment, url, integration_text );
}

void vent_add_channel( int channelid ) {
	channels_add( channelid );
	printf( "here\n" );
	v3_channel* chan = channels_get( channelid );

	if ( chan == NULL ) {
		printf( "chan is null\n" );
		return;
	}

	printf( "channel: %s ( %s )\n", chan->name, chan->comment );
}

void vent_user_login( int userid, int channelid ) {
	printf( "channel: %i, usr: %i\n", channelid, userid );
	users_add( userid );

	v3_channel* chan = channels_get( channelid );
	v3_user* user = users_get( userid );

	if ( chan == NULL ) {
		printf( "channl is null\n" );
		return;
	}

	if ( user == NULL ) {
		printf( "user is null\n " );
		return;
	}
	
	printf( "user login: %s (%s) joined %s ( %s )\n", user->name, user->comment, chan->name, chan->comment );
}

void vent_remove_channel( int channelid ) {

}

