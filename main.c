/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"
#include "b_tree.h"


int main() {
  FILE *bin_file = open_file("TESTE", "wb+");
  btree_t *btree = create_btree(4);
  btree_insert(btree, 'C', 100);
  btree_insert(btree, 'S', 100);
  btree_insert(btree, 'D', 100);
  btree_insert(btree, 'T', 100);
  btree_insert(btree, 'A', 100);
  btree_insert(btree, 'M', 100);
  btree_insert(btree, 'P', 100);
  btree_insert(btree, 'I', 100);
  btree_insert(btree, 'B', 100);
  btree_insert(btree, 'W', 100);
  btree_insert(btree, 'N', 100);
  btree_insert(btree, 'G', 100);
  btree_insert(btree, 'U', 100);
  btree_insert(btree, 'R', 100);
  btree_insert(btree, 'K', 100);

  btree_index_header_t *header = create_btree_index_header(btree);

  write_index_header(bin_file, header);
  write_index_node(bin_file, btree, btree->root);

  fclose(bin_file);
  bin_file = open_file("TESTE", "r");
  btree_index_header_t *header2 = read_index_header(bin_file);
  fseek(bin_file, 77, SEEK_SET);
  node_t *node = create_disk_node(0, NULL, btree->order, true);
  node_t *node2 = read_index_node(bin_file, btree->order, NULL, node);
  fclose(bin_file);
  print_by_level(btree);
//  parse_input();
  return 0;
}
