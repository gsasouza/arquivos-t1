/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "queue.h"

queue_node_t *dequeue(queue_t *queue) {
  queue_node_t *node = queue->start;
  queue->start = node->next;
  return node;
}

void enqueue(queue_t *queue, queue_node_t *node) {
  if (queue->start == NULL) {
    queue->start = node;
    queue->end = node;
    return;
  }
  node->prev = queue->end;
  queue->end->next = node;
  queue->end = node;
}

queue_node_t *create_queue_node(void *data) {
  queue_node_t *node = malloc(sizeof(queue_node_t));
  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

queue_t *create_queue() {
  queue_t *queue = malloc(sizeof(queue_t));
  queue->end = NULL;
  queue->start = NULL;
  return queue;
}