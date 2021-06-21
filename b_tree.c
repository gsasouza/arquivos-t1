/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */
#include "b_tree.h"

void btree_insert_internal(node_t *node, record_t *record, int order);

/**
 * create new btree node
 * @param parent - parent node
 * @param order - order
 * @param is_leaf - is a leaf node
 * @return - created node
 */
node_t *create_node(node_t *parent, int order, bool is_leaf) {
  node_t *node = malloc(sizeof(node_t));
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
btree_t *create_b_tree(int order) {
  btree_t *b_tree = malloc(sizeof(btree_t));
  b_tree->order = order;
  b_tree->root = create_node(NULL, order, true);
  return b_tree;
}

/**
 * create a new record, a pair of key and value
 * @param key
 * @param value
 * @return - created record
 */
record_t *create_record(int key, void *value) {
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
bool find_key_in_node(node_t *node, int key) {
  for (int i = 0; i < node->n_keys; i++) {
    if (node->records[i]->key == key) return true;
  }
  return false;
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
  }
}

/**
 * splits node with overflow in two new nodes
 * @param node - current node
 * @param order - btree order
 */
void split_node(node_t *node, int order) {
  node_t *parent = node->parent;

  // root split is handled in another function
  if (parent == NULL) return;

  // create new nodes
  node_t *left = node;
  node_t *right = create_node(parent, order, left->is_leaf);;

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

  // if parent now has an overflow, call split recursively for parent
  if (parent->n_keys == order) return split_node(parent, order);
}

/**
 * insert new key in a leaf node
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_leaf(node_t *node, record_t *record, int order) {

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

  // if it has an overflow and is not root then split the node
  if (node->n_keys == order && node->parent != NULL) {
    split_node(node, order);
  }
}

/**
 * traverse the tree searching a leaf node to insert new record
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_nonleaf(node_t *node, record_t *record, int order) {
  int position = node->n_keys - 1;

  // key already exits, just end insertion
  if (find_key_in_node(node, record->key)) return;

  // find position
  while (position >= 0 && record->key < node->records[position]->key) {
    position--;
  }
  position++;

  // call insert recursively
  btree_insert_internal(node->children[position], record, order);
}

/**
 * Internal driver to decide if we need to search node or we are already in a node and can insert the record
 * @param node - current node
 * @param record - record to be inserted
 * @param order - btree order
 */
void btree_insert_internal(node_t *node, record_t *record, int order) {
  if (node->is_leaf) return btree_insert_leaf(node, record, order);
  return btree_insert_nonleaf(node, record, order);
}

/**
 * Splits a btree root
 * @param btree
 */
void split_root_overflow(btree_t *btree) {
  // start new nodes
  node_t *left = btree->root;
  node_t *new_root = create_node(NULL, btree->order, false);
  node_t *right = create_node(new_root, btree->order, left->is_leaf);
  left->parent = new_root;
  new_root->children[0] = btree->root;
  new_root->children[1] = right;


  // divide old root
  int half = ceil((btree->order - 1) / 2.0);
  left->n_keys = half;
  right->n_keys = btree->order - 1 - half;

  // promote key
  new_root->n_keys = 1;
  new_root->records[0] = left->records[half];

  // move keys to right side;
  for (int i = 0; i < right->n_keys; i++) {
    right->records[i] = left->records[half + i + 1];
  }
  // move children to right side
  for (int i = 0; i <= right->n_keys; i++) {
    right->children[i] = left->children[half + i + 1];
  }

  // change root
  btree->root = new_root;
}

/**
 * Add a new record in btree
 * @param btree
 * @param key - record key
 * @param value - record value
 */
void btree_insert(btree_t *btree, int key, void *value) {
  record_t *record = create_record(key, value);
  btree_insert_internal(btree->root, record, btree->order);
  // if it has an overflow, split root
  if (btree->root->n_keys == btree->order) split_root_overflow(btree);
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
  if (btree->root == NULL) {
    printf("Empty tree.\n");
    return;
  }

  enqueue(queue, create_queue_node(btree->root));
  printf("| ");
  while (queue->start != NULL) {
    current = dequeue(queue);
    btree_node = current->data;

    int node_level = calculate_height_from_node(btree, btree_node);
    if (node_level > level) {
      level = node_level;
      printf("\n| ");
    }
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

