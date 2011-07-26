#include "channels.h" 
#include "users.h"

void vent_init();
void vent_add_channel( int channelid );
void vent_remove_channel( int channelid );
void vent_user_login( int userid, int channelid );
void vent_display_motd( int userid, char* comment, char* url, char* integration_text );
