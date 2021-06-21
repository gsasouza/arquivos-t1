/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"
#include "b_tree.h"


int main() {
  btree_t *btree = create_b_tree(4);
  btree_insert(btree, 'C', (void *) 100);
  btree_insert(btree, 'S', (void *) 100);
  btree_insert(btree, 'D', (void *) 100);
  btree_insert(btree, 'T', (void *) 100);
  btree_insert(btree, 'A', (void *) 100);
  btree_insert(btree, 'M', (void *) 100);
  btree_insert(btree, 'P', (void *) 100);
  btree_insert(btree, 'I', (void *) 100);
  btree_insert(btree, 'B', (void *) 100);
  btree_insert(btree, 'W', (void *) 100);
  btree_insert(btree, 'N', (void *) 100);
  btree_insert(btree, 'G', (void *) 100);
  btree_insert(btree, 'U', (void *) 100);
  btree_insert(btree, 'R', (void *) 100);
  btree_insert(btree, 'K', (void *) 100);
  print_by_level(btree);
//  parse_input();
  return 0;
}
