/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#ifndef T1_B_TREE_H
#define T1_B_TREE_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include "queue.h"
#include "helpers.h"

#define ORDER 4
#define DISK_PAGE_SIZE 77
#define BYTE_OFFSET(rnn) (rnn + 1) * DISK_PAGE_SIZE

/**
 * key, value pair that will be stored
 */
typedef struct record {
  char key;
  long value;
} record_t;

/**
 * btree node
 */
typedef struct node {
  int rrn; // rnn for this node
  bool is_leaf; // is leaf node
  bool is_loaded; // store if node needs to be loaded from file
  int n_keys; // records stored in this node
  record_t **records; // records data
  struct node **children; // node children
  struct node *parent; // node parent
  /*Refactor*/
  int parent_rrn;
  int children_rrn[ORDER];
  record_t records2[ORDER];
} node_t;

typedef struct btree_index_header {
  int status;
  int root_node_rrn;
  int next_node_rrn;
} btree_index_header_t;

void btree_insert(FILE* file,  btree_index_header_t *header,int key, long value);

void print_by_level_from_disk(FILE* file);

record_t *btree_find_node(node_t *node, int key);

void write_index_header(FILE* file, btree_index_header_t* header);

void write_index_node(FILE* file, node_t *node);

btree_index_header_t *read_index_header(FILE *file);

node_t *read_index_node(FILE *file, int rrn, node_t* parent);

btree_index_header_t *create_btree_index_header();

record_t *btree_find_node_disk(FILE* file, btree_index_header_t *header, node_t* node, int key);

#endif //T1_B_TREE_H
