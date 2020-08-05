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
	// TODO: Check for NULL pointer
	p_node_t thisNode = (p_node_t)calloc(1, sizeof(node_t));
	thisNode->item = newItem;
	thisNode->next = NULL;
	return thisNode;
}

static inline void addNode( p_node_t list, ll_type_t newItem )
{
	// TODO: Check for NULL pointer
	p_node_t tail = list;
	while( tail->next != NULL ) tail = tail->next;
	tail->next = createNode( newItem );
}

static inline int countItems( p_node_t list )
{
	// TODO: Check for NULL pointer
	p_node_t _list = list;
	int count = 1;
	while( _list->next != NULL )
	{
		_list = _list->next;
		count++;
	}
	return count;
}

static inline ll_type_t getItem( p_node_t list, int index )
{
	// TODO: Check for NULL pointer
	p_node_t _list = list;
	for( index; index > 0; index-- ) _list = _list->next;
	return _list->item;
}


#endif // LINKED_LIST_H