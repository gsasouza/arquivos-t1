/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"
#include "b_tree.h"


int main() {
  FILE *bin_file = open_file("TESTE", "wb+");
  btree_index_header_t *header = init_index_file(bin_file);
  btree_insert(bin_file, header, 'C', 100);
  btree_insert(bin_file, header, 'S', 100);
  btree_insert(bin_file, header, 'D', 100);
  btree_insert(bin_file, header, 'T', 100);
  btree_insert(bin_file, header, 'A', 100);
//  btree_insert(bin_file, header, 'M', 100);
//  btree_insert(bin_file, header, 'P', 100);
//  btree_insert(bin_file, header, 'I', 100);
//  btree_insert(bin_file, header, 'B', 100);
//  btree_insert(bin_file, header, 'W', 100);
//  btree_insert(bin_file, header, 'N', 100);
//  btree_insert(bin_file, header, 'G', 100);
//  btree_insert(bin_file, header, 'U', 100);
//  btree_insert(bin_file, header, 'R', 100);
//  btree_insert(bin_file, header, 'K', 100);
//  btree_insert(bin_file, header, 'B', 100);
//  btree_insert(bin_file, header, 'E', 100);
//  btree_insert(bin_file, header, 'F', 100);
//  btree_insert(bin_file, header, 'H', 100);
//  btree_insert(bin_file, header, 'J', 100);
//  btree_insert(bin_file, header, 'L', 100);
//  btree_insert(bin_file, header, 'O', 100);
//  btree_insert(bin_file, header, 'V', 100);
//  btree_insert(bin_file, header, 'X', 100);
//  btree_insert(bin_file, header, 'Z', 100);
//  btree_insert(bin_file, header, 'Y', 100);

  fclose(bin_file);
  bin_file = open_file("TESTE", "r");
  print_in_order(bin_file);
//  parse_input();
  return 0;
}
