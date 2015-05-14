/*
 * list_mtm1.c
 *
 *  Created on: May 6, 2015
 *      Author: or
 */
#include "list_mtm1.h"
#include <assert.h>
#include <stdlib.h>



typedef struct node_t{
	ListElement data;
	struct node_t* next;
} *Node;


struct List_t {
	Node first;
	Node current;
	CopyListElement copyElement;
	FreeListElement freeElement;
};

static void putNodeBeforeCurrent(List list, Node node);

static void listSetPreivious(List list, Node current);

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
	if (!copyElement || !freeElement) {
		return NULL;
	}
	List newList = calloc(1, sizeof(*newList));
	if (!newList) {
		return NULL;
	}

	newList->copyElement = copyElement;
	newList->freeElement = freeElement;
	return newList;
}

List listCopy(List list) {
	if (!list) {
		return NULL;
	}
	Node current = list->current;
	List newList = listCreate(list->copyElement, list->freeElement);
	if (!newList) {
		return NULL;
	}
	listInsertFirst(newList, listGetFirst(list));

	while (list->current != NULL) {
		ListResult insertStatus = listInsertLast(newList, listGetNext(list));
		if (insertStatus != LIST_SUCCESS) {
			listDestroy(newList);
			return NULL;
		}
	}
	if (current == NULL) {
		list->current = NULL;
		newList->current = NULL;
	} else {
		int i = 0, size = listGetSize(list);
		listGetFirst(list);
		while (list->current != current && i < size) {
			i++;
			listGetNext(list);
		}
		listGetFirst(newList);
		for (; i >= 0 && i < size; --i) {
			listGetNext(newList);
		}
	}
	return newList;
}

int listGetSize(List list) {
	if (!list) {
		return -1;
	}
	Node temp = list->current;
	listGetFirst(list);
	int count = 1;
	while (listGetNext(list)) {
		count++;
	}
	list->current = temp;
	return count;
}

ListElement listGetFirst(List list) {
	if (!list) {
		return NULL;
	}
	list->current = list->first;
	return listGetCurrent(list);
}

ListElement listGetNext(List list) {
	if (!list) {
		return NULL;
	}
	if (!(list->current) || !(list->current->next)) {
		return NULL;
	}
	list->current = list->current->next;
	return listGetCurrent(list);
}

ListResult listInsertFirst(List list, ListElement element) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	Node temp = list->current;
	Node newFirst = malloc(sizeof(*newFirst));
	if (!newFirst) {
		return LIST_OUT_OF_MEMORY;
	}
	newFirst->data = list->copyElement(element);
	if (!(newFirst->data)) {
		free(newFirst);
		return LIST_OUT_OF_MEMORY;
	}
	newFirst->next = list->first;
	list->first = newFirst;
	list->current = temp;
	return (LIST_SUCCESS);
}

static void putNodeBeforeCurrent(List list, Node node) {
	assert(list && node);
	listGetFirst(list);
	while (node->next != list->current->next) {
		listGetNext(list);
	}
	list->current->next = node;
	listGetNext(list); //iterator on added node
	listGetNext(list); //Iterator on original node
	return;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	if (list->current == list->first) {
		return listInsertFirst(list, element);
	}
	if (list->current == NULL) {
		return LIST_INVALID_CURRENT; //FIXME is it? maybe listInsertLast?
	}
	Node newNode = malloc(sizeof(*newNode));
	if (!newNode) {
		return LIST_OUT_OF_MEMORY;
	}
	newNode->data = list->copyElement(element);
	if (!(newNode->data)) {
		free(newNode);
		return LIST_OUT_OF_MEMORY;
	}

	newNode->next = list->current;
	putNodeBeforeCurrent(list, newNode);
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	if(!(list->current)){
		return LIST_INVALID_CURRENT;
	}
	Node removedNode = list->current;
	if (list->current != list->first) {
		listSetPreivious(list, removedNode);
		list->current->next = removedNode->next;
	} else {
		list->first = list->first->next;
	}
	list->freeElement(removedNode->data);
	free(removedNode);
	list->current = NULL;
	return LIST_SUCCESS;
}

static void listSetPreivious(List list, Node current) {
	assert(list && current);
	listGetFirst(list);
	while (current != list->current->next) {
		listGetNext(list);
	}
	return;
}

ListResult listClear(List list) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	listGetFirst(list); //set iterator on first
	while (listRemoveCurrent(list) == LIST_SUCCESS) {
	};
	list->first = NULL;

	return LIST_SUCCESS;
}

