#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main( int argc, char** argv ) {
	struct list_s *list = list_create( "hello" );
	list_insert( list, "world" );
	list_insert( list, "this" );
	list_insert( list, "is" );
	list_insert( list, "a" );
	list_insert( list, "test" );
	
	list_remove( list, "is" );
	list_remove( list, "hello" );
	list_remove( list, "test" );
	printf( "hello\n" );
	list_insert( list, "fuck" );
	list_insert( list, "you" );
	list_remove( list, "you" );
	list_traverse( list );
	
}
