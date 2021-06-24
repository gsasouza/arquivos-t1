/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"
#include "b_tree.h"


int main() {
  FILE *bin_file = open_file("TESTE", "wb+");
  btree_t *btree = create_btree(4);
  btree_insert(bin_file, btree, 'C', 100);
  btree_insert(bin_file, btree, 'S', 100);
  btree_insert(bin_file, btree, 'D', 100);
  btree_insert(bin_file,btree, 'T', 100);
  btree_insert(bin_file,btree, 'A', 100);
  btree_insert(bin_file,btree, 'M', 100);
  btree_insert(bin_file,btree, 'P', 100);
  btree_insert(bin_file,btree, 'I', 100);
  btree_insert(bin_file,btree, 'B', 100);
  btree_insert(bin_file,btree, 'W', 100);
  btree_insert(bin_file,btree, 'N', 100);
  btree_insert(bin_file,btree, 'G', 100);
  btree_insert(bin_file,btree, 'U', 100);
  btree_insert(bin_file,btree, 'R', 100);
  btree_insert(bin_file,btree, 'K', 100);

  btree_index_header_t *header = create_btree_index_header(btree);
  write_index_header(bin_file, header);
  fclose(bin_file);
  bin_file = open_file("TESTE", "r");
//  btree_index_header_t *header2 = read_index_header(bin_file);
//  node_t *node = create_disk_node(header2->root_node_rrn, NULL, btree->order, true);
//  node_t *node2 = read_index_node(bin_file, btree->order, NULL, node);
//  fclose(bin_file);
  print_by_level(btree);
  printf("\n");
  print_by_level_from_disk(bin_file);
//  parse_input();
  return 0;
}
