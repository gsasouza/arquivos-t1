/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#ifndef T1_QUEUE_H
#define T1_QUEUE_H

#include <stdlib.h>

typedef struct queue_node {
  void *data;
  struct queue_node *next;
  struct queue_node *prev;
} queue_node_t;

typedef struct queue {
  queue_node_t *start;
  queue_node_t *end;
} queue_t;

queue_node_t *dequeue(queue_t *queue);

void enqueue(queue_t *queue, queue_node_t *node);

queue_node_t *create_queue_node(void *data);

queue_t *create_queue();

#endif //T1_QUEUE_H
