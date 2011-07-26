#ifndef _CHANNELS_H
#define _CHANNELS_H

#include <stdio.h>
#include <stdlib.h>
#include <ventrilo3.h>
#include "settings.h"

#define CHANNELS_MAX 1024

int channel_count;
int channel_limit;

typedef struct _channel {
	v3_channel *chan_ptr;
	
	struct _channel *child;
	struct _channel *next;
} vent_channel;

void channels_add( int channelid );
void channels_remove( int channelid );
v3_channel* channels_get( int channelid );
void channels_update( int channelid );
void channels_calc_limit();

#endif
