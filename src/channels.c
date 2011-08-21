#include "channels.h"

void channels_add( int channelid ) {
	
	// exit if we haven't initialized the channel stuff
	
	// get channel data from libventrilo
	v3_channel* p_chan = v3_get_channel( channelid );

	// dont add if we dont have data to add.
	if ( p_chan == NULL ) {
		printf( "trying to add null channel\n" );
		return;
	}

	channels[channelid] = p_chan;	
	
	//channel* chan = malloc( sizeof( struct channel_s ) );
	//chan->data = chan_ptr;

	//	INIT_LIST_HEAD( &chan->sub );
//		INIT_LIST_HEAD( &chan->list );
//		list_add( &(chan->list), &(channel_root.list) );
	
	
	
	printf( "added channel: %s\n", p_chan->name );

	//while ( channels[i] != NULL ) i++;
	
	//printf( "putting channel (%s,%s)[%i|%i] in %i\n", chan_ptr->name, chan_ptr->comment, chan_ptr->id, chan_ptr->parent, i );

	//channels[i] = chan_ptr;
	//if ( i > channel_limit ) channel_limit = i;
}

void channels_remove( int channelid ) {

	v3_channel* p_chan = channels_get( channelid );
	
	printf( "removing %s\n", p_chan->name );

	v3_free_channel( p_chan );
	channels[channelid] = NULL;
}

v3_channel* channels_get( int channelid ) {
	
	return channels[channelid];
	
	/*
	int i;
	v3_channel *chan_ptr = NULL;
	
	if ( has_init != 1210 ) {
		printf( "NO GETchannels not init'd\n" );
		return chan_ptr;
	}

	for( i = 0; i <= channel_limit; i++ ) {
		if ( channels[i] == NULL ) continue;
		if ( channels[i]->id == channelid )
			chan_ptr = channels[i];
	}

	return chan_ptr;
	*/
}

void channels_update( int channelid ) {
	// update all channels
	for( int i = 0; i < MAX_CHANNELS; i++ ) {
		if ( channels[i] != NULL ) {
			v3_free_channel( channels[i] );
			channels[i] = v3_get_channel( i );
		}
	}
}

int channels_has( int channelid ) { 

	return ( channels[channelid] != NULL );
	/*struct list_head *pos;

	if ( has_init == 0 ) {
		printf( "channels not init'd\n" );
		return 0;
	}

	v3_channel* chan = channel_root.data;
	if ( chan == NULL )
		printf( "channel is nullllll\n" );

	printf( "root channel info: %s\n%s", chan->name, chan->comment );
		
	printf( "listing channels\n" );
	//struct list_head* pos;
	struct channel_s* tmp;

	list_for_each( pos, &channel_root.list ) {
		tmp = list_entry( pos, struct channel_s, list );
		v3_channel* chan = tmp->data;
		printf( "%s\n", chan->name );
	}
	
	return 1;
	*/
}
