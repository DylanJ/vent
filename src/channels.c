#include "channels.h"

v3_channel* channels[CHANNELS_MAX];

void channels_init() {
	int i;
	channel_count = 0;
	channel_limit = 0;

	// make sure everything is null.
	for( i = 0; i < CHANNELS_MAX; i++ ) {
		channels[i] = NULL;
	}
}

void channels_add( int channelid ) {

	int i = 0;
	v3_channel* chan_ptr = v3_get_channel( channelid );
	
	if ( chan_ptr != NULL )
		printf( "channel being added\n" );
	else {
		printf( "channel being added is null\n" );
	}

	while ( channels[i] != NULL ) i++;
	
	printf( "putting channel (%s,%s)[%i|%i] in %i\n", chan_ptr->name, chan_ptr->comment, chan_ptr->id, chan_ptr->parent, i );

	channels[i] = chan_ptr;
	channel_count += 1;
	
	if ( i > channel_limit ) channel_limit = i;
}

void channels_remove( int channelid ) {
	v3_channel* chan_ptr = channels_get( channelid );
	channel_count -= 1;
	
	printf( "removing %s\n", chan_ptr->name );

	v3_free_channel( chan_ptr );
}

v3_channel* channels_get( int channelid ) {
	
	int i;
	v3_channel *chan_ptr = NULL;
	
	for( i = 0; i <= channel_limit; i++ ) {
		if ( channels[i] == NULL ) continue;
		if ( channels[i]->id == channelid )
			chan_ptr = channels[i];
	}

	return chan_ptr;
}

void channels_update( int channelid ) {

}
