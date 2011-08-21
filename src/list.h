#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>

struct list_s {
	void *data;
	struct list_s *next;
};

struct list_s *list_create( void *data );
void list_insert( struct list_s *list, void *data );
void list_remove( struct list_s *list, void *data );
void list_contains( struct list_s *list, void *data );

#endif
