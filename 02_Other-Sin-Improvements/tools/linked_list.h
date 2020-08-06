#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include "error.h"

typedef double ll_type_t;

typedef struct node
{
	ll_type_t item;
	struct node * next;
} node_t, * p_node_t;

static inline p_node_t createNode( ll_type_t newItem )
{
	p_node_t thisNode = (p_node_t)calloc(1, sizeof(node_t));
	if( thisNode != NULL )
	{
		thisNode->item = newItem;
		thisNode->next = NULL;
	}
	
	return thisNode;
}

static inline errno_t addNode( p_node_t list, ll_type_t newItem )
{
	errno_t err = 0;

	if( list == NULL ) err = -1;

	if( err == 0 )
	{
		p_node_t tail = list;
		while( tail->next != NULL ) tail = tail->next;
		tail->next = createNode( newItem );
		if( tail->next == NULL ) err = -1;
	}

	return err;	
}

static inline errno_t countItems( p_node_t list, int * p_count )
{
	errno_t err = 0;

	if( list == NULL ) err = -1;
	if( p_count == NULL ) err = -1;

	if( err == 0 )
	{
		p_node_t _list = list;
		int count = 1;
		while( _list->next != NULL )
		{
			_list = _list->next;
			count++;
		}
		*p_count = count;
	}

	return err;
}

static inline errno_t getItem( p_node_t list, int index, ll_type_t * p_item )
{
	errno_t err = 0;
	p_node_t _list;

	if( list == NULL ) err = -1;
	if( p_item == NULL ) err = -1;

	if( err == 0 )
	{
		_list = list;
		for( index; index > 0; index-- ) _list = _list->next;
		*p_item = _list->item;
	}
	
	return err;
}


#endif // LINKED_LIST_H