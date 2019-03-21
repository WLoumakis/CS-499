/*
 * Program written by: Walt Loumakis
 *
 * SLL is a singly-linked list with common functions for a list. 
 *
 */

#include "sll.h"
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	void *data;
	struct node *next;
}Node;

struct sll {
	int size;
	Node *head, *tail;
	void (*display)(void *, FILE *);
	void (*free)(void *);
};

SLL *newSLL(void (*d)(void *, FILE *), void (*f)(void *)) {
	SLL *items = (SLL*)malloc(sizeof(SLL));
	assert(items != 0);
	items->head = 0;
	items->tail = 0;
	items->size = 0;
	items->display = d;
	items->free = f;
	return items;
}

void insertSLL(SLL *items, int index, void *value) {
	assert(index >= 0 && index <= items->size);

	Node *n = (Node*)malloc(sizeof(Node));
	n->data = value;

		//If the list is empty, sets the head and tail to the new node.
	if (items->size == 0) {
		items->head = items->tail = n;
		n->next = 0;
		items->size += 1;
		return;
	}

		//If the node is to be inserted at the end, use tail.
	if (index == items->size) {
		items->tail->next = n;
		n->next = 0;
		items->tail = n;
		items->size += 1;
		return;
	}

		//If the node is to be inserted at the beginning, use head.
	if (index == 0) {
		n->next = items->head;
		items->head = n;
		items->size += 1;
		return;
	}
		//If none of the above 3 cases hold, use the generic insert found below.
	Node *p = items->head;
	for (int i = 0; i < index - 1; i++)
		p = p->next;
	n->next = p->next;
	p->next = n;
	items->size += 1;
}

void *removeSLL(SLL *items, int index) {
	assert(items->size > 0);
	assert(index >= 0 && index < items->size);

		//Case if the item to be removed is the head.
	if(index == 0) {
		Node *r = items->head;
		void *ret = r->data;
		items->head = items->head->next;
		if (items->size == 1) items->tail = 0;
		items->size -= 1;
		free(r);
		return ret;
	}

		//Case if the item to be removed is not the head.
	Node *p = items->head;
	for (int i = 0; i < index - 1; i++)
		p = p->next;
	Node *r = p->next;
	void *ret = r->data;
	p->next = r->next;
	items->size -= 1;
	free(r);
	return ret;
}

void unionSLL(SLL *recipient, SLL *donor) {
	if (donor->size == 0) return;			//Takes care of donor being empty (recipient can be empty or non-empty).
	if (recipient->size == 0) {				//Takes care of recipient empty and donor non-empty (from above).
		recipient->head = donor->head;
		recipient->tail = donor->tail;
		recipient->size = donor->size;
		donor->head = donor->tail = 0;
		donor->size = 0;
		return;
	}
	recipient->tail->next = donor->head;	//Takes care of neither list being empty.
	recipient->tail = donor->tail;
	donor->head = donor->tail = 0;
	recipient->size += donor->size;
	donor->size = 0;
	return;
}

void *getSLL(SLL *items, int index) {
	assert(index >= 0 && index < items->size);
		//Case where index is head.
	if (index == 0)
		return items->head->data;

		//Case where index is tail.
	if (index == items->size - 1)
		return items->tail->data;

		//Case where index is neither head nor tail.
	Node *p = items->head;
	for (int i = 0; i < index; i++)
		p = p->next;
	return p->data;
}

void *setSLL(SLL *items, int index, void *value) {
	assert(index >= 0 && index <= items->size);

		//Case if size is zero.
	if (items->size == index) {
		insertSLL(items, items->size, value);
		return 0;
	}

		//Case if index is the head of the list.
	if (index == 0) {
		void *r = items->head->data;
		items->head->data = value;
		return r;
	}

		//Case if index is the tail of the list.
	if (index == items->size - 1) {
		void *r = items->tail->data;
		items->tail->data = value;
		return r;
	}

		//Generic case for if the above cases are false.
	Node *p = items->head;
	for (int i = 0; i < index; i++)
		p = p->next;
	void *r = p->data;
	p->data = value;
	return r;
}

int sizeSLL(SLL *items) {
	return items->size;
}

void displaySLL(SLL *items, FILE *fp) {
	Node *p = items->head;
	fprintf(fp, "{");
	int tf = (int)items->size > 0;
	while (tf != 0 && p->next != 0) {
		items->display(p->data, fp);
		fprintf(fp, ",");
		p = p->next;
	}
	if (items->size > 0) items->display(p->data, fp);
	fprintf(fp, "}");
}

void displaySLLdebug(SLL *items, FILE *fp) {
	Node *p = items->head;
	fprintf(fp, "head->{");
	int tf = (int)items->size > 0;
	while (tf != 0 && p->next != 0) {
		items->display(p->data, fp);
		fprintf(fp, ",");
		p = p->next;
	}
	if (items->size > 0) items->display(p->data, fp);
	fprintf(fp, "},tail->{");
	if (items->size > 0) items->display(items->tail->data, fp);
	fprintf(fp, "}");
}

void freeSLL(SLL *items) {
	Node *p = items->head;
	while (items->head != 0) {
		p = items->head;
		items->head = items->head->next;
		if (items->free != NULL) items->free(p->data);
		free(p);
	}
	items->size = 0;
	free(items);
}
