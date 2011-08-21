#ifndef _CHANNELS_H
#define _CHANNELS_H

#include <stdio.h>
#include <stdlib.h>
#include <ventrilo3.h>
#include "list.h"
#include "settings.h"

#define MAX_CHANNELS 1024

int channel_count;
int channel_limit;

v3_channel* channels[MAX_CHANNELS];

void channels_add( int channelid );
void channels_remove( int channelid );
v3_channel* channels_get( int channelid );
void channels_update( int channelid );
int  channels_has( int cid );

#endif
