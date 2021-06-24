/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"
#include "b_tree.h"


int main() {
  FILE *bin_file = open_file("TESTE", "wb+");
  btree_t *btree = create_btree(ORDER);
  btree_index_header_t *header = create_btree_index_header(btree);
  write_index_header(bin_file, header);
  btree_insert(bin_file, btree, header, 'C', 100);
  btree_insert(bin_file, btree, header, 'S', 100);
  btree_insert(bin_file, btree, header, 'D', 100);
  btree_insert(bin_file, btree, header, 'T', 100);
  btree_insert(bin_file, btree, header, 'A', 100);
  btree_insert(bin_file, btree, header, 'M', 100);
  btree_insert(bin_file, btree, header, 'P', 100);
  btree_insert(bin_file, btree, header, 'I', 100);
  btree_insert(bin_file, btree, header, 'B', 100);
  btree_insert(bin_file, btree, header, 'W', 100);
  btree_insert(bin_file,btree,header, 'N', 100);
  btree_insert(bin_file,btree,header, 'G', 100);
  btree_insert(bin_file,btree, header,'U', 100);
  btree_insert(bin_file,btree,header, 'R', 100);
  btree_insert(bin_file,btree,header, 'K', 100);


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
