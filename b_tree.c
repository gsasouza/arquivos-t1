/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */
#include "b_tree.h"


void btree_insert_internal(FILE *file, btree_index_header_t *header, node_t *node, record_t *record);

/**
 * create a new record, a pair of key and value
 * @param key
 * @param value
 * @return - created record
 */
record_t *create_record(int key, long value) {
  record_t *record = malloc(sizeof(record_t));
  record->key = key;
  record->value = value;
  return record;
}

/**
 * find key in a given node
 * @param node
 * @param key
 * @return - key found or not
 */
record_t *find_key_in_node(node_t *node, int key) {
  for (int i = 0; i < node->n_keys; i++) {
    if (node->records[i].key == key) return &node->records[i];
  }
  return NULL;
}

/**
 * helper to move records in ascending order
 * @param to
 * @param from
 * @param begin
 * @param end
 * @param padding_to
 * @param padding_from
 */
void move_records_asc(node_t *to, node_t *from, int begin, int end, int padding_to, int padding_from) {
  for (int i = begin; i < end; i++) {
    to->records[i + padding_to] = from->records[i + padding_from];
  }
}

/**
 * helper to move records in descending order
 * @param to
 * @param from
 * @param begin
 * @param end
 * @param padding_to
 * @param padding_from
 */
void move_records_desc(node_t *to, node_t *from, int begin, int end, int padding_to, int padding_from) {
  for (int i = begin; i > end; i--) {
    to->records[i + padding_to] = from->records[i + padding_from];
  }
}

/**
 * Helper to move children in ascending order
 * @param to
 * @param from
 * @param begin
 * @param end
 * @param padding_to
 * @param padding_from
 */
void move_children_asc(node_t *to, node_t *from, int begin, int end, int padding_to, int padding_from) {
  for (int i = begin; i < end; i++) {
    to->children_rrn[i + padding_to] = from->children_rrn[i + padding_from];
  }
}

/**
 * Helper to move children in descending order
 * @param to
 * @param from
 * @param begin
 * @param end
 * @param padding_to
 * @param padding_from
 */
void move_children_desc(node_t *to, node_t *from, int begin, int end, int padding_to, int padding_from) {
  for (int i = begin; i > end; i--) {
    to->children_rrn[i + padding_to] = from->children_rrn[i + padding_from];
  }
}

node_t *create_node(int rnn, node_t *parent, bool is_leaf) {
  node_t *node = malloc(sizeof(node_t));
  node->rrn = rnn;
  node->n_keys = 0;
  node->is_leaf = is_leaf;
  node->parent = parent;
  node->child = NULL;
  return node;
}

/**
 * splits node with overflow in two new nodes
 * @param node - current node
 * @param order - btree order
 */
void split_node(FILE *file, btree_index_header_t *header, node_t *node) {
  node_t *parent = node->parent;
  // root split is handled in another function
  if (parent == NULL) return;

  // create new nodes
  node_t *left = node;
  header->next_node_rrn++;
  node_t *right = create_node(header->next_node_rrn, node, left->is_leaf);
  // divide old node
  int half = ceil((ORDER - 1) / 2.0);
  left->n_keys = half;
  right->n_keys = ORDER - 1 - half;

  // move keys from left to right
  move_records_asc(right, left, 0, right->n_keys, 0, half + 1);
  if (!left->is_leaf) {
    move_children_asc(right, left, 0, right->n_keys + 1, 0, half + 1);
  }

  // find position to promote key
  int position = parent->n_keys - 1;
  int promoted_key = left->records[half].key;
  while (position >= 0 && promoted_key < parent->records[position].key) {
    position--;
  }
  position++;

  // open space in parent for new key
  move_records_desc(parent, parent, parent->n_keys - 1, position - 1, 1, 0);
  move_children_desc(parent, parent, parent->n_keys, position, 1, 0);

  // insert promoted key in parent
  parent->n_keys++;
  parent->records[position] = left->records[half];
  parent->children_rrn[position + 1] = right->rrn;

  // write to file
  write_index_node(file, left);
  write_index_node(file, right);

  // if parent now has an overflow, call split recursively for parent, else write results to file
  if (parent->n_keys == ORDER) return split_node(file, header, parent);
  else write_index_node(file, parent);

  free(left);
  free(right);
}

/**
 * insert new key in a leaf node
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_leaf(FILE *file, btree_index_header_t *header, node_t *node, record_t *record) {

  // key already exists, just end insertion
  if (find_key_in_node(node, record->key)) return;

  // open space in node for new key and find new key position
  int position = node->n_keys - 1;
  while (position >= 0 && record->key < node->records[position].key) {
    node->records[position + 1] = node->records[position];
    position--;
  }
  position++;

  // insert new record
  node->records[position] = *record;
  node->n_keys++;

  // if it has an overflow and is not root then split the node
  if (node->n_keys == ORDER && node->parent != NULL) {
    split_node(file, header, node);
  } else write_index_node(file, node);
}

/**
 * traverse the tree searching a leaf node to insert new record
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_nonleaf(FILE *file, btree_index_header_t *header, node_t *node, record_t *record) {
  int position = node->n_keys - 1;

  // key already exits, just end insertion
  if (find_key_in_node(node, record->key)) return;

  // find position
  while (position >= 0 && record->key < node->records[position].key) {
    position--;
  }
  position++;

  node->child = read_index_node(file, node->children_rrn[position], node);
  // call insert recursively
  btree_insert_internal(file, header, node->child, record);
}

/**
 * Internal driver to decide if we need to search node or we are already in a node and can insert the record
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_internal(FILE *file, btree_index_header_t *header, node_t *node, record_t *record) {
  if (node->is_leaf) return btree_insert_leaf(file, header, node, record);
  return btree_insert_nonleaf(file, header, node, record);
}

/**
 * Splits a btree root
 * @param btree
 */
void split_root(FILE *file, btree_index_header_t *header, node_t *root_node) {
  // start new nodes
  node_t *left = root_node;
  header->next_node_rrn++;
  node_t *right = create_node(header->next_node_rrn, root_node, left->is_leaf);
  header->next_node_rrn++;
  node_t *new_root = create_node(header->next_node_rrn, NULL, false);

  // assign splitted nodes to new root
  right->parent = new_root;
  left->parent = new_root;
  new_root->children_rrn[0] = root_node->rrn;
  new_root->children_rrn[1] = right->rrn;

  // divide old root
  int half = ceil((ORDER - 1) / 2.0);
  left->n_keys = half;
  right->n_keys = ORDER - 1 - half;

  // promote key
  new_root->n_keys = 1;
  new_root->records[0] = left->records[half];

  // move keys and children to right side;
  move_records_asc(right, left, 0, right->n_keys, 0, half + 1);
  move_children_asc(right, left, 0, right->n_keys + 1, 0, half + 1);

  // write to file

  write_index_node(file, left);
  write_index_node(file, right);
  write_index_node(file, new_root);

  // change root
  header->root_node_rrn = new_root->rrn;
  write_index_header(file, header);

  // clean up
  free(left);
  free(right);
  free(new_root);
}

/**
 * Add a new record in btree
 * @param btree
 * @param key - record key
 * @param value - record value
 */
void btree_insert(FILE *file, btree_index_header_t *header, int key, long value) {
  record_t *record = create_record(key, value);
  node_t *root_node = read_index_node(file, header->root_node_rrn, NULL);
  btree_insert_internal(file, header, root_node, record);
  // if it has an overflow, split the root root, else release root_node
  if (root_node->n_keys == ORDER) split_root(file, header, root_node);
  else free(root_node);
  free(record);
}

void print_by_level(FILE *file) {
  queue_node_t *current;
  queue_t *queue = create_queue();
  btree_index_header_t *header = read_index_header(file);
  node_t *btree_node = read_index_node(file, header->root_node_rrn, NULL);
  if (btree_node->n_keys == 0) {
    printf("Empty tree.\n");
    return;
  }

  enqueue(queue, create_queue_node(btree_node));
  printf("| ");
  while (queue->start != NULL) {
    current = dequeue(queue);
    btree_node = current->data;
    printf("(%d) ", btree_node->rrn);
    for (int i = 0; i < btree_node->n_keys; i++) {
      printf("%d ", btree_node->records[i].key);
    }
    printf("| ");
    if (!btree_node->is_leaf) {
      for (int i = 0; i <= btree_node->n_keys; i++) {
        enqueue(queue, create_queue_node(read_index_node(file, btree_node->children_rrn[i], btree_node)));
      }
    }
  }
}

btree_index_header_t *create_btree_index_header() {
  btree_index_header_t *header = malloc(sizeof(btree_index_header_t));
  header->status = true;
  header->root_node_rrn = 0;
  header->next_node_rrn = 0;
  return header;
}


record_t *btree_find_node(FILE *file, btree_index_header_t *header, node_t *node, int key) {
  // found
  record_t *record = find_key_in_node(node, key);
  if (record) return record;

  // stop condition: not found and is a leaf node
  if (node->is_leaf) return NULL;

  // find position
  int position = node->n_keys - 1;
  while (position >= 0 && key < node->records[position].key) {
    position--;
  }
  position++;

  //load next node from file before calling function recursively
  node->child = read_index_node(file, node->children_rrn[position], node);
  return btree_find_node(file, header, node->child, key);
}

/**
 * write index header to file
 * @param file
 * @param header
 */
void write_index_header(FILE *file, btree_index_header_t *header) {
  char empty_pointer = '@';
  char status = format_status_bool(header->status);
  fseek(file, 0, SEEK_SET);
  fwrite(&status, 1, 1, file);
  fwrite(&header->root_node_rrn, 4, 1, file);
  fwrite(&header->next_node_rrn, 4, 1, file);
  fwrite(&empty_pointer, 1, 68, file);
}

/**
 * write btree index node to file
 * @param file
 * @param btree
 * @param node
 */
void write_index_node(FILE *file, node_t *node) {
  int empty_pointer = -1;
  char is_leaf = format_status_bool(node->is_leaf);
  // nkeys = 3
  // 1 + 4 + 4 +
  fseek(file, BYTE_OFFSET(node->rrn), SEEK_SET);
  fwrite(&is_leaf, 1, 1, file);
  fwrite(&node->n_keys, 4, 1, file);
  fwrite(&node->rrn, 4, 1, file);

  // write records and pointers
  if (node->is_leaf) fwrite(&empty_pointer, 4, 1, file);
  else fwrite(&node->children_rrn[0], 4, 1, file);

  for (int i = 0; i < node->n_keys; i++) {
    fwrite(&node->records[i].key, 4, 1, file);
    fwrite(&node->records[i].value, 8, 1, file);
    fwrite(&node->children_rrn[i + 1], 4, 1, file);
  }

  for (int i = node->n_keys; i < ORDER - 1; i++) {
    fwrite(&empty_pointer, 4, 1, file);
    fwrite(&empty_pointer, 8, 1, file);
    fwrite(&empty_pointer, 4, 1, file);
  }
}

/**
 *
 * @param file
 */
btree_index_header_t *read_index_header(FILE *file) {
  fseek(file, 0, SEEK_SET);
  char buffer;
  btree_index_header_t *header = malloc(sizeof(btree_index_header_t));
  fread(&buffer, 1, 1, file);
  header->status = buffer == '0' ? false : true;
  fread(&header->root_node_rrn, 4, 1, file);
  fread(&header->next_node_rrn, 4, 1, file);
  return header;
}

node_t *read_index_node(FILE *file, int rrn, node_t *parent) {
  node_t *node = create_node(rrn, parent, false);
  fseek(file, BYTE_OFFSET(rrn), SEEK_SET);
  char buffer;
  fread(&buffer, 1, 1, file);
  node->is_leaf = buffer == '0' ? false : true;
  node->parent = parent;
  fread(&node->n_keys, 4, 1, file);
  fread(&node->rrn, 4, 1, file);

  fread(&node->children_rrn[0], 4, 1, file);
  for (int i = 0; i < node->n_keys; i++) {
    fread(&node->records[i].key, 4, 1, file);
    fread(&node->records[i].value, 8, 1, file);
    fread(&node->children_rrn[i + 1], 4, 1, file);
  }
  // @TODO remove this
  node->child = malloc(sizeof(node_t *));
  return node;
}

btree_index_header_t *init_index_file(FILE *file) {
  fseek(file, 0, SEEK_SET);
  btree_index_header_t *header = create_btree_index_header();
  header->status = false;
  write_index_header(file, header);
  return header;
}

void print_in_order_internal(FILE *file, node_t *node) {
  if (!node->is_leaf) print_in_order_internal(file, read_index_node(file, node->children_rrn[0], NULL));

  for (int i = 0; i < node->n_keys; i++) {
    printf("%d \n", node->records[i].key);
    if (!node->is_leaf) print_in_order_internal(file, read_index_node(file, node->children_rrn[i + 1], NULL));

  }

}

void print_in_order(FILE *file) {
  btree_index_header_t *header = read_index_header(file);
  return print_in_order_internal(file, read_index_node(file, header->root_node_rrn, NULL));
}