#include "list.h"

struct list_s *list_create( void *data ) {
	struct list_s *list = malloc( sizeof( struct list_s ) );
	list->data = data;
	list->next = NULL;

	printf( "inserting %s\n", data );

	return list;
} 

void list_insert( struct list_s *list, void *data ) {
	if ( list->next == NULL )
		list->next = list_create( data );
	else
		list_insert( list->next, data );
}

void list_traverse( struct list_s *list ) {
	if ( list->data != NULL ) {
		printf( ": %s\n", (char*)list->data );
	}

	if ( list->next != NULL )
		list_traverse( list->next );
}

void list_remove( struct list_s *_list, void *data ) {
	if ( _list == NULL )
		return;
		
	struct list_s *list, *prev;
	
	list = _list;


	while( list != NULL ) {
		
		if( strcmp( list->data, data ) ) {
			if ( list->next == NULL ) {
				printf( "\n" );
				prev->next = NULL;
			} else {
				printf( "b\n" );
				prev->next = list->next;
			}

			break;
		}

		prev = list;
		list = list->next;
	}

	printf( "end of remove\n" );
 }
	
/*
	// match found
	//if ( list->data == data ) {
	if ( strcmp(list->data, data)==0 ) {
		// let user free data.
		if ( list->next == NULL ) {
			list->data = NULL;
			list->next = NULL;
			list = NULL;
		} else {
			list->data = list->next->data;
			list->next = list->next->next;
		}

		return;
	}

	list_remove( list->next, data );
	*/
