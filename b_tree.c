/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */
#include "b_tree.h"

/****** RAM functions ******/

void btree_insert_internal(FILE* file, btree_t *btree, node_t *node, record_t *record, int order);

/**
 * create new btree node
 * @param parent - parent node
 * @param order - order
 * @param is_leaf - is a leaf node
 * @return - created node
 */
node_t *create_node(btree_t* btree, node_t *parent, int order, bool is_leaf) {
  node_t *node = malloc(sizeof(node_t));
  node->rrn = btree->n_records + 1;
  btree->n_records = btree->n_records + 1;
  node->n_keys = 0;
  node->is_leaf = is_leaf;
  node->children = malloc((order + 1) * sizeof(node_t *));
  node->records = malloc((order) * sizeof(record_t *));
  node->parent = parent;
  return node;
}


/**
 * create new btree
 * @param order - btree order
 * @return - created btree
 */
btree_t *create_btree(int order) {
  btree_t *btree = malloc(sizeof(btree_t));
  btree->n_records = -1;
  btree->order = order;
  btree->root = create_node(btree, NULL, order, true);
  return btree;
}

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
    if (node->records[i]->key == key) return node->records[i];
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
    to->children[i + padding_to] = from->children[i + padding_from];
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
    to->children[i + padding_to] = from->children[i + padding_from];
    to->children_rrn[i + padding_to] = from->children_rrn[i + padding_from];
  }
}

/**
 * splits node with overflow in two new nodes
 * @param node - current node
 * @param order - btree order
 */
void split_node(FILE* file, btree_t *btree, node_t *node, int order) {
  node_t *parent = node->parent;

  // root split is handled in another function
  if (parent == NULL) return;

  // create new nodes
  node_t *left = node;
  node_t *right = create_node(btree, parent, order, left->is_leaf);;

  // divide old node
  int half = ceil((order - 1) / 2.0);
  left->n_keys = half;
  right->n_keys = order - 1 - half;

  // move keys from left to right
  move_records_asc(right, left, 0, right->n_keys, 0, half + 1);
  if (!left->is_leaf) {
    move_children_asc(right, left, 0, half, 0, half);
  }

  // find position to promote key
  int position = parent->n_keys - 1;
  int promoted_key = left->records[half]->key;
  while (position >= 0 && promoted_key < parent->records[position]->key) {
    position--;
  }
  position++;

  // open space in parent for new key
  move_records_desc(parent, parent, parent->n_keys - 1, position - 1, 1, 0);
  move_children_desc(parent, parent, parent->n_keys, position, 1, 0);

  // insert promoted key in parent
  parent->n_keys++;
  parent->records[position] = left->records[half];
  parent->children[position + 1] = right;

  // write to file
  write_index_node(file, btree, parent);
  write_index_node(file, btree, left);
  write_index_node(file, btree, right);

  // if parent now has an overflow, call split recursively for parent
  if (parent->n_keys == order) return split_node(file, btree, parent, order);
}

/**
 * insert new key in a leaf node
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_leaf(FILE *file, btree_t *btree, node_t *node, record_t *record, int order) {

  // key already exists, just end insertion
  if (find_key_in_node(node, record->key)) return;

  // open space in node for new key and find new key position
  int position = node->n_keys - 1;
  while (position >= 0 && record->key < node->records[position]->key) {
    node->records[position + 1] = node->records[position];
    position--;
  }
  position++;

  // insert new record
  node->records[position] = record;
  node->n_keys++;
  write_index_node(file, btree, node);

  // if it has an overflow and is not root then split the node
  if (node->n_keys == order && node->parent != NULL) {
    split_node(file, btree, node, order);
  }
}

/**
 * traverse the tree searching a leaf node to insert new record
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_nonleaf(FILE* file, btree_t *btree, node_t *node, record_t *record, int order) {
  int position = node->n_keys - 1;

  // key already exits, just end insertion
  if (find_key_in_node(node, record->key)) return;

  // find position
  while (position >= 0 && record->key < node->records[position]->key) {
    position--;
  }
  position++;

  // call insert recursively
  btree_insert_internal(file, btree, node->children[position], record, order);
}

/**
 * Internal driver to decide if we need to search node or we are already in a node and can insert the record
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_internal(FILE* file, btree_t *btree, node_t *node, record_t *record, int order) {
  if (node->is_leaf) return btree_insert_leaf(file, btree, node, record, order);
  return btree_insert_nonleaf(file, btree, node, record, order);
}

/**
 * Splits a btree root
 * @param btree
 */
void split_root_overflow(FILE* file, btree_t *btree) {
  // start new nodes
  node_t *left = btree->root;
  node_t *right = create_node(btree, NULL, btree->order, left->is_leaf);
  node_t *new_root = create_node(btree, NULL, btree->order, false);
  right->parent = new_root;
  right->parent_rrn = new_root->rrn;
  left->parent = new_root;
  left->parent_rrn = new_root->rrn;
  new_root->children[0] = btree->root;
  new_root->children_rrn[0] = btree->root->rrn;
  new_root->children[1] = right;
  new_root->children_rrn[1] = right->rrn;


  // divide old root
  int half = ceil((btree->order - 1) / 2.0);
  left->n_keys = half;
  right->n_keys = btree->order - 1 - half;

  // promote key
  new_root->n_keys = 1;
  new_root->records[0] = left->records[half];

  // move keys and children to right side;
  move_records_asc(right, left, 0, right->n_keys, 0, half + 1);
  move_children_asc(right, left, 0, right->n_keys + 1, 0, half + 1);

  // write to file
  write_index_node(file, btree, new_root);
  write_index_node(file, btree, left);
  write_index_node(file, btree, right);

  // change root
  btree->root = new_root;
}

/**
 * Add a new record in btree
 * @param btree
 * @param key - record key
 * @param value - record value
 */
void btree_insert(FILE* file, btree_t *btree, int key, long value) {
  record_t *record = create_record(key, value);
  btree_insert_internal(file, btree, btree->root, record, btree->order);
  // if it has an overflow, split root
  if (btree->root->n_keys == btree->order) split_root_overflow(file, btree);
}

int calculate_height_from_node(btree_t *btree, node_t *node) {
  int height = 0;
  node_t *current = node;
  while (current->parent) {
    height++;
    current = current->parent;
  }
  return height;
}

void print_by_level(btree_t *btree) {
  int level = 0;
  node_t *btree_node;
  queue_node_t *current;
  queue_t *queue = create_queue();
  if (btree->root->n_keys == 0) {
    printf("Empty tree.\n");
    return;
  }

  enqueue(queue, create_queue_node(btree->root));
  printf("| ");
  while (queue->start != NULL) {
    current = dequeue(queue);
    btree_node = current->data;

    int node_level = calculate_height_from_node(btree, btree_node);
//    if (node_level > level) {
//      level = node_level;
//      printf("\n| ");
//    }
    printf("(%d) ", btree_node->rrn);
    for (int i = 0; i < btree_node->n_keys; i++) {
      printf("%c ", btree_node->records[i]->key);
    }
    printf("| ");

    if (!btree_node->is_leaf) {
      for (int i = 0; i <= btree_node->n_keys; i++) {
        enqueue(queue, create_queue_node(btree_node->children[i]));
      }
    }
  }
}

record_t *btree_find_node(node_t *node, int key) {
  if (!node) return NULL;

  record_t *record = find_key_in_node(node, key);
  if (record) return record;

  // find position
  int position = node->n_keys - 1;
  while (position >= 0 && key < node->records[position]->key) {
    position--;
  }
  position++;
  return btree_find_node(node->children[position], key);
}

/****** Disk functions ******/

void print_by_level_from_disk(FILE *file) {
  queue_node_t *current;
  queue_t *queue = create_queue();
  btree_index_header_t *header = read_index_header(file);
  node_t *btree_node = read_index_node(file, header->root_node_rrn, -1);
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
      printf("%c ", btree_node->records2[i].key);
    }
    printf("| ");
    if (!btree_node->is_leaf) {
      for (int i = 0; i <= btree_node->n_keys; i++) {
        enqueue(queue, create_queue_node(read_index_node(file, btree_node->children_rrn[i],  btree_node->rrn)));
      }
    }
  }
}

btree_index_header_t *create_btree_index_header(btree_t *btree) {
  btree_index_header_t *header = malloc(sizeof( btree_index_header_t));
  header->status = true;
  header->root_node_rrn = btree->root->rrn;
  header->next_node_rrn = btree->n_records;
  return header;
}

node_t *create_disk_node(int rnn, node_t *parent, int order, bool is_leaf) {
  node_t *node = malloc(sizeof(node_t));
  node->rrn = rnn;
  node->n_keys = 0;
  node->is_leaf = is_leaf;
  node->is_loaded = false;
  node->children = malloc((order + 1) * sizeof(node_t *));
  node->records = malloc((order) * sizeof(record_t *));
  node->parent = parent;
  return node;
}

record_t *find_key_in_node_disk(node_t *node, int key) {
  for (int i = 0; i < node->n_keys; i++) {
    if (node->records2[i].key == key) return &node->records2[i];
  }
  return NULL;
}


record_t *btree_find_node_disk(FILE* file, btree_index_header_t *header, node_t* node, int key) {
  // found
  record_t *record = find_key_in_node_disk(node, key);
  if (record)  return record;

  // stop condition: not found
  if (node->is_leaf) return NULL;



  // find position
  int position = node->n_keys - 1;
  while (position >= 0 && key < node->records2[position].key) {
    position--;
  }
  position++;

  //load next node from file before calling function recursively
  node_t *next_node = read_index_node(file, node->children_rrn[position], node->rrn);
  return btree_find_node_disk(file, header, next_node, key);
}

/**
 * write index header to file
 * @param file
 * @param header
 */
void write_index_header(FILE* file, btree_index_header_t* header) {
  fseek(file, 0, SEEK_SET);
  fwrite(format_status_bool(header->status), 1, 1, file);
  fwrite(&header->root_node_rrn, 4, 1, file);
  fwrite(&header->next_node_rrn, 4, 1, file);
  fwrite("@", 1, 68, file);
}

/**
 * write btree index node to file
 * @param file
 * @param btree
 * @param node
 */
void write_index_node(FILE* file, btree_t *btree, node_t *node){
  fseek(file, BYTE_OFFSET(node->rrn), SEEK_SET);
  int null_pointer = -1;
  fwrite(format_status_bool(node->is_leaf), 1, 1, file);
  fwrite(&node->n_keys, 4, 1, file);
  fwrite(&node->rrn, 4, 1, file);

  // write records and pointers
  if (node->is_leaf) fwrite(&null_pointer, 4, 1, file);
  else fwrite(&node->children[0]->rrn, 4, 1, file);

  for (int i = 0; i < node->n_keys; i++) {
    fwrite(&node->records[i]->key, 4, 1, file);
    fwrite(&node->records[i]->value, 8, 1, file);
    if (node->is_leaf) fwrite(&null_pointer, 4, 1, file);
    else fwrite(&node->children[i + 1]->rrn, 4, 1, file);
  }

  // write empty records and pointers
  for (int i = node->n_keys; i < btree->order; i++) {
    fwrite(&null_pointer, 4, 1, file);
    fwrite(&null_pointer, 8, 1, file);
    fwrite(&null_pointer, 4, 1, file);
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

node_t *read_index_node(FILE *file, int rrn, int parent_rrn) {
  node_t *node = malloc(sizeof(node_t));
  fseek(file, BYTE_OFFSET(rrn), SEEK_SET);
  char buffer;
  fread(&buffer, 1, 1, file);
  node->is_leaf = buffer == '0' ? false : true;
  node->parent_rrn = parent_rrn;
  fread(&node->n_keys, 4, 1, file);
  fread(&node->rrn, 4, 1, file);

  fread(&node->children_rrn[0], 4, 1, file);
  for (int i = 0; i < node->n_keys; i++) {
    fread(&node->records2[i].key, 4, 1, file);
    fread(&node->records2[i].value, 8, 1, file);
    fread(&node->children_rrn[i + 1], 4, 1, file);
  }
  return node;
}

