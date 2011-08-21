#include "channels.h" 
#include "users.h"

struct vent_tree_node_s {
	v3_channel *data; 	// channel data
	struct list_s channels;
	struct list_s users;
};

void vent_init();

/* channel stuff */
void vent_channel_add( uint16_t channelid );
void vent_channel_remove( uint16_t channelid );
void vent_channel_modify( uint16_t channelid );

/* user stuff */
void vent_user_add( uint16_t userid );
void vent_user_remove( uint16_t userid );
void vent_user_talk_start( uint16_t userid, uint16_t send_type, uint32_t rate );
void vent_user_talk_end( uint16_t userid );
void vent_user_login( uint16_t userid, uint16_t channelid );
void vent_user_move( uint16_t userid, uint16_t channelid );
void vent_user_modify( uint16_t userid, char* comment, char* url, char* integration_text );

void vent_display_motd( uint16_t userid, char* comment, char* url, char* integration_text );
