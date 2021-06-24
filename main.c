/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"
#include "b_tree.h"


int main() {
  FILE *bin_file = open_file("TESTE", "wb+");
  btree_index_header_t *header = create_btree_index_header();
  write_index_header(bin_file, header);
  btree_insert(bin_file, header, 'C', 100);
  btree_insert(bin_file, header, 'S', 100);
  btree_insert(bin_file, header, 'D', 100);
  btree_insert(bin_file, header, 'T', 100);
  btree_insert(bin_file, header, 'A', 100);
  btree_insert(bin_file, header, 'M', 100);
  btree_insert(bin_file, header, 'P', 100);
  btree_insert(bin_file, header, 'I', 100);
  btree_insert(bin_file, header, 'B', 100);
  btree_insert(bin_file, header, 'W', 100);
  btree_insert(bin_file, header, 'N', 100);
  btree_insert(bin_file, header, 'G', 100);
  btree_insert(bin_file, header, 'U', 100);
  btree_insert(bin_file, header, 'R', 100);
  btree_insert(bin_file, header, 'K', 100);


//  write_index_header(bin_file, header);
  fclose(bin_file);
  bin_file = open_file("TESTE", "r");
  header = read_index_header(bin_file);
  print_by_level_from_disk(bin_file);
  node_t *root = read_index_node(bin_file, header->root_node_rrn, NULL);
  record_t *record = btree_find_node_disk(bin_file, header, root, 'X');
//  parse_input();
  return 0;
}
