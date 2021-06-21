/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#ifndef T1_B_TREE_H
#define T1_B_TREE_H
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "queue.h"

/**
 * key, value pair that will be stored
 */
typedef struct record {
  int key;
  void *value;
} record_t;

/**
 * btree node
 */
typedef struct node {
  bool is_leaf; // is leaf node
  int n_keys; // records stored in this node
  record_t **records; // records data
  struct node **children; // node children
  struct node *parent; // node parent
} node_t;

typedef struct btree {
  int order; // btree order
  node_t *root; // root pointer
} btree_t;


btree_t *create_b_tree(int order);

void btree_insert(btree_t *btree, int key, void* value);

void print_by_level(btree_t *btree);


#endif //T1_B_TREE_H
