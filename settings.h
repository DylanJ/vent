#ifndef _SETTINGS_H
#define _SETTINGS_H

typedef struct _settings {
	char*	hostname; 	// server hostname
	char* 	name;		// login name
	char*	phonetic;	// login phonetic

	int 	max_users;
	int		port;
} vent_settings;

vent_settings settings;
#endif
