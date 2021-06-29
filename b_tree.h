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
#include "helpers.h"

#define ORDER 5
#define DISK_PAGE_SIZE 77
#define BYTE_OFFSET(rnn) (((rnn) + 1) * DISK_PAGE_SIZE)

/**
 * key, value pair that will be stored
 */
typedef struct record {
  int key;
  long value;
} record_t;

/**
 * btree node
 */
typedef struct node {
  int rrn; // rnn for this node
  bool is_leaf; // is leaf node
  int n_keys; // number of records stored in this node
  struct node *child; // store child node so we can do a backtrack
  struct node *parent; // store parent node so we can do a backtrack
  record_t records[ORDER]; // store records data in this node
  int children_rrn[ORDER]; // store children rrn for this node
} node_t;

/**
 * index header
 */
typedef struct btree_index_header {
  bool status; // header status
  int root_node_rrn; // rrn of root node
  int next_node_rrn; // store how many nodes we have in the index
} btree_index_header_t;

void btree_insert(FILE* file,  btree_index_header_t *header,int key, long value);

void print_in_order(FILE* file);

void write_index_header(FILE* file, btree_index_header_t* header);

void write_index_node(FILE* file, node_t *node);

btree_index_header_t *read_index_header(FILE *file);

node_t *read_index_node(FILE *file, int rrn, node_t* parent);

btree_index_header_t *create_btree_index_header();

record_t *btree_find_node(FILE* file, btree_index_header_t *header, node_t* node, int key);

btree_index_header_t *init_index_file(FILE *file);

#endif //T1_B_TREE_H
