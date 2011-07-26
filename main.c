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

#include "channels.h"
#include "vent.h"

#define USER "test"
#define SERVER "vent.hellacute.net:4722"

// prototypes
void help();
void ctrl_c( int signum );
void* jukebox_connection( void *connptr );
void* jukebox_player( void *connptr );

int debug = 2;
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
				printf( "event user channel move\n" );
				break;
			case V3_EVENT_CHAN_ADD:
				vent_add_channel( ev->channel.id );
				break;
			case V3_EVENT_CHAN_MODIFY:
				//event.channel.id
				printf( "event channel modify\n" );
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
				break;

			case V3_EVENT_USER_TALK_END:
				//event.user.id
				printf( "event user talk end\n" );
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
				printf( "event login complete\n" );
				break;
		}
		free(ev);
	}
	pthread_exit(NULL);
}


int main( int argc, char** argv ) {
	int rc = 0;
	
	debug = 1;
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
