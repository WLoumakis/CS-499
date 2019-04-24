/*
 * Program written by: Walt Loumakis
 *
 * QUEUE is based on a singly-linked list with common functions for a queue. 
 *
 */

#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "sll.h"

struct queue {
  SLL *sll;
  void (*display)(void *, FILE *);
  void (*free)(void*);
};

QUEUE *newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)) {
  QUEUE *q = malloc(sizeof(QUEUE));
  assert(q != 0);
  q->display = d;
  q->free = f;
  q->sll = newSLL(d, f);
  return q;
}

void enqueue(QUEUE *items,void *value) {
  insertSLL(items->sll, sizeQUEUE(items), value);
}

void *dequeue(QUEUE *items) {
  return removeSLL(items->sll, 0);
}

void *peekQUEUE(QUEUE *items) {
  return getSLL(items->sll, 0);
}

int sizeQUEUE(QUEUE *items) {
  return sizeSLL(items->sll);
}

void displayQUEUE(QUEUE *items,FILE *fp) {
  fprintf(fp, "<");
  for (int i = 0; i < sizeQUEUE(items); i++) {
    items->display(getSLL(items->sll, i), fp);
    if (i < sizeQUEUE(items) - 1) fprintf(fp, ",");
  }
  fprintf(fp, ">");
}

void displayQUEUEdebug(QUEUE *items,FILE *fp) {
  displaySLLdebug(items->sll, fp);
}

void freeQUEUE(QUEUE *items) {
  freeSLL(items->sll);
  free(items);
}
