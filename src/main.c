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

#include "users.h"
#include "channels.h"
#include "vent.h"

#define USER "test"
#define SERVER "vent.hellacute.net:4722"

// prototypes
void help();
void draw( int root, int depth );
void draw_user( v3_user* user, int depth );
void draw_channel( v3_channel* channel, int depth );
void ctrl_c( int signum );
void* jukebox_connection( void *connptr );
void* jukebox_player( void *connptr );

int debug = 0;
int should_exit = 0;

struct _conninfo {
	char *server;
	char *username;
	char *password;
	char *channelid;
	char *path;
};

void ctrl_c( int signum ) {
	printf( "disconnect\n" );
	v3_logout();
	printf( "done\n" );
	exit(EXIT_SUCCESS);
}

void *jukebox_connection( void* connptr ) {
	struct _conninfo *conninfo;
	_v3_net_message *msg;
	conninfo = connptr;

	if( debug >= 2 ) {
		v3_debuglevel( V3_DEBUG_ALL );
	}

	if ( !v3_login( conninfo->server, conninfo->username, conninfo->password, "" ) ) {
		printf( "couldn't login: %s\n", _v3_error(0) );
	}

	while( ( msg = _v3_recv( V3_BLOCK ) ) != NULL ) {
		switch( _v3_process_message( msg ) ) {
			case V3_MALFORMED:
				break;
			case V3_NOTIMPL:
				break;
			case V3_OK:
				break;
		}
	}

	should_exit = 1;
	pthread_exit( NULL );
}

void *jukebox_player( void *connptr ) {
	struct _conninfo *conninfo;
	v3_event *ev;

	conninfo = connptr;
	while( ( ev = v3_get_event( V3_BLOCK ) ) ) {
		if ( debug ) {
			printf( "jukebox: got event type %d\n", ev->type );
		}
	
		switch( ev->type ) {
			case 40:
				vent_init();
				break;
			case V3_EVENT_STATUS:
				//event.status.percent
				//event.status.text
				printf( "event: %s, %i\n", ev->status.message, ev->status.percent );
				break;
			case V3_EVENT_PING:
				//event.ping
				printf( "event ping\n" );
				break;
			case V3_EVENT_USER_LOGIN:
				//event.user.id
				//event.channel.id
				vent_user_login( ev->user.id, ev->channel.id );
				break;
			case V3_EVENT_USER_CHAN_MOVE:
				//event.user.id
				//event.channel.id
				vent_user_move( ev->user.id, ev->channel.id );
				printf( "event user channel move\n" );
				draw(0, 1);
				break;
			case V3_EVENT_CHAN_ADD:
				//
				vent_channel_add( ev->channel.id );
				printf( "event chan add\n" );
				break;
			case V3_EVENT_CHAN_MODIFY:
				//event.channel.id
				printf( "event channel modify\n" );
				vent_channel_modify( ev->channel.id );

				draw(0, 1);
				break;
			case V3_EVENT_CHAN_REMOVE:
				//event.channel.id
				printf( "event channel remove\n" );
				break;
			case V3_EVENT_CHAN_BADPASS:
				//event.channel.id
				//event.error.message
				printf( "event channel bad pass\n" );
				break;
			case V3_EVENT_ERROR_MSG:
				//event.error.message
				//event.error.disconnected
				printf( "event error message\n" );
				break;
			case V3_EVENT_USER_TALK_START:
				//event.user.id
				//event.pcm.send_type
				//event.pcm.rate
				printf( "event user talk start\n" );
				vent_user_talk_start( ev->user.id, ev->pcm.send_type, ev->pcm.rate );
				draw(0, 1);
				break;

			case V3_EVENT_USER_TALK_END:
				//event.user.id
				printf( "event user talk end\n" );
				vent_user_talk_end( ev->user.id );	
				draw(0, 1);
				break;

			case V3_EVENT_USER_MODIFY:
				vent_user_modify( ev->user.id, ev->text.comment,
					ev->text.url, ev->text.integration_text );
				printf( "event user modify\n" );
				draw(0, 1);
				break;

			case V3_EVENT_USER_LOGOUT:
				vent_user_logout( ev->user.id, ev->channel.id );
				printf( "event user modify\n" );
				draw( 0, 1 );
				
				break;
			case V3_EVENT_PLAY_AUDIO:
				//event.user.id
				//event.pcm.send_type
				//event.pcm.rate
				//event.pcm.length
				//event.data.sample
				//event.data.sample16
				printf( "event play audio\n" );
				break;
			case V3_EVENT_DISPLAY_MOTD:
				//ev.user.id
				//ev.text.comment
				//ev.text.url
				//ev.text.integration_text
				vent_display_motd( ev->user.id, ev->text.comment, ev->text.url, ev->text.integration_text );
				break;
			case V3_EVENT_CHAT_JOIN:
				//ev.user.id
				//ev.data.chatmessage
				draw(0,1);
				printf( "event chat join\n" );
				break;

			case V3_EVENT_PHANTOM_ADD:
				//ev.user.id
				//ev.channel.id
				printf( "event phantom add\n" );
				break;
			case V3_EVENT_PHANTOM_REMOVE:
				//ev.user.id
				//ev.channel.id
				printf( "event phantom remove\n" );
				break;
			case V3_EVENT_ADMIN_AUTH:
				//local user is now admin
				printf( "event admin auth\n" );
				break;
			case V3_EVENT_CHAN_ADMIN_UPDATED:
				//local users admin rights for channels changed
				printf( "event chan admin updated\n" );
				break;
			case V3_EVENT_USERLIST_ADD:
				//ev.account.id
				printf( "event userlist add\n" );
				break;
			case V3_EVENT_USERLIST_REMOVE:
				//ev.account.id
				printf( "event userlist remove\n" );
				break;
			case V3_EVENT_USERLIST_MODIFY:
				//ev.account.id
				printf( "event userlist modify\n" );
				break;
			case V3_EVENT_USERLIST_CHANGE_OWNER:
				printf( "userlist change owner\n" );
				break;
			case V3_EVENT_CHANGE_CHANNEL:
				printf( "channel change\n" );
			case V3_EVENT_DISCONNECT:
				should_exit = 1;
				free( ev );
				pthread_exit( NULL );
				break;
			case V3_EVENT_LOGIN_COMPLETE:
				v3_change_channel( atoi( conninfo->channelid), "" );

				v3_join_chat();
				channels_has(2);
				printf( "event login complete\n" );
				break;
		}
		free(ev);
	}
	pthread_exit(NULL);
}

void drawtabs( int num ) {
	for( int i = 0; i < num; i++ ) {
		printf( "  " );
	}
}

void draw_channel( v3_channel* channel, int depth ) {

	char msg[256];

	sprintf( msg, "%s", channel->name );

	if ( strlen( channel->comment ) > 0 ) {
		sprintf( msg, "%s (%s)", msg, channel->comment );
	}

	drawtabs( depth );
	printf( "%s\n", msg );
}

void draw_user( v3_user* user, int depth ) {
	// user is lobby
	char msg[256];
	
	if ( user->bitfield & 0x02 ) {
		
		sprintf( msg, "%s", user->name );
		if ( strlen(user->comment) > 0 ) {
			sprintf( msg, "%s (%s)\n", msg, user->comment );
		}
		
		printf( msg );
		return;
	}

	sprintf( msg, "" );
	
	if ( user->bitfield & 0x400 ) {
		strcat( msg, "[G]" );
	}

	if ( user->is_transmitting ) {
		strcat( msg, ":<" );
	}

	strcat( msg, user->name );

	if ( strlen(user->comment) > 0 ) {
		strcat( msg, "(" );
		strcat( msg, user->comment );
		strcat( msg, ")" );
	}
	
	if ( strlen(user->url) > 0 ) {
		strcat( msg, "(" );
		strcat( msg, user->url );
		strcat( msg, ")" );
	}

	if ( strlen( user->integration_text ) > 0 ) {
		strcat( msg, "(" );
		strcat( msg, user->integration_text );
		strcat( msg, ")" );
	}
	
	drawtabs( depth );
	printf( "%s\n", msg );	
//	printf( "%s (%s)(%s)\n", user->name, user->comment, user->url);
}

void draw(int root, int depth) {
	
	for( int i = 0; i < 256; i++ ) {
		if ( users[i] == NULL )
			continue;
		if ( users[i]->channel == root ) {
			draw_user( users[i], depth );
		}
	}
	
	for( int i = 0; i < MAX_CHANNELS; i++ ) {
		if ( channels[i] == NULL )
			continue;
		if ( root == channels[i]->parent ) {
			
			draw_channel( channels[i], depth );	
			draw( channels[i]->id, depth + 1 );

		}
	}
/*
	for( int i = 0; i < 256; i++ ) {
		if ( users[i] == NULL )
			continue;
		if ( users[i]->channel == root ) {
			drawtabs(depth);
			printf( "u: %s\n", users[i]->name );
		}
	}
*/

}




int main( int argc, char** argv ) {
	int rc = 0;
	
	pthread_t network;
	pthread_t player;

	struct _conninfo conninfo;

	conninfo.server = SERVER;
	conninfo.username = USER;
	conninfo.password = "";
	conninfo.channelid = "";

	conninfo.path = strdup( argv[0] );

	rc = pthread_create( &network, NULL, jukebox_connection, (void *)&conninfo );
	rc = pthread_create( &player, NULL, jukebox_player, (void *)&conninfo );
	
	printf( "rc: %i\n", rc );

	signal( SIGINT, ctrl_c );

	while( !should_exit ) {
		sleep( 1 );
	}

	return 0;
}
