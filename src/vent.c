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

	printf( "max clients: %i\n", settings.max_users );
}

/* * * * * *  * 
 * user stuff *
 * * * * * *  */
void vent_user_talk_start( uint16_t userid, uint16_t send_type, uint32_t rate ) {
	v3_user* p_user = users_get( userid );
	p_user->is_transmitting = 1;
}

void vent_user_talk_end( uint16_t userid ) {
	v3_user* p_user = users_get( userid );
	p_user->is_transmitting = 0;
}

void vent_user_move( uint16_t userid, uint16_t channelid ) {
	v3_user* p_user = users_get( userid );
	p_user->channel = channelid;
}

void vent_user_modify( uint16_t userid, char* commnent, char* url, char* integration_text ) {
	
	users_update( userid );

}

void vent_user_login( uint16_t userid, uint16_t channelid ) {
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

void vent_user_logout( uint16_t userid, uint16_t channelid ) {
	users_remove( userid );
}

/* * * * * * * * * 
 * channel stuff *
 * * * * * * * * */
void vent_display_motd( uint16_t userid, char* comment,
	char* url, char* integration_text ) {
	printf( "MOTD id: %i, %s, %s, %s\n", userid, comment, url, integration_text );
}

void vent_channel_add( uint16_t channelid ) {
	printf( "adding channel (%i)\n", channelid );
	channels_add( channelid );
	v3_channel* chan = channels_get( channelid );

	if ( chan == NULL ) {
		printf( "chan is null\n" );
		return;
	}

	printf( "channel: %s ( %s )\n", chan->name, chan->comment );
}

void vent_channel_remove( uint16_t channelid ) {

}

void vent_channel_modify( uint16_t channelid ) {
	channels_update( channelid );
}
