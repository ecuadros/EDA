#ifndef __DEMO_H__
#define __DEMO_H__

void DemoArray();
void DemoBinaryTree();
void DemoHash();

#endif // __DEMO_H__

void DemoArray() {
  // Create an array of integers.
  int array[] = {1, 2, 3, 4, 5};

  // Iterate over the array and print the elements.
  for (int i = 0; i < sizeof(array) / sizeof(int); i++) {
    printf("%d\n", array[i]);
  }
}

void DemoBinaryTree() {
  // Create a binary tree.
  struct Node {
    int value;
    struct Node* left;
    struct Node* right;
  };

  struct Node* root = malloc(sizeof(struct Node));
  root->value = 10;

  root->left = malloc(sizeof(struct Node));
  root->left->value = 5;

  root->right = malloc(sizeof(struct Node));
  root->right->value = 15;

  // Iterate over the binary tree and print the values.
  struct Node* node = root;
  while (node != NULL) {
    printf("%d\n", node->value);

    if (node->left != NULL) {
      node = node->left;
    } else if (node->right != NULL) {
      node = node->right;
    } else {
      node = NULL;
    }
  }
}

void DemoHash() {
  // Create a hash table.
  struct HashTable {
    int* values;
    int size;
  };

  struct HashTable* hashTable = malloc(sizeof(struct HashTable));
  hashTable->values = malloc(sizeof(int) * 10);
  hashTable->size = 10;

  // Insert some values into the hash table.
  hashTable->values[0] = 1;
  hashTable->values[1] = 2;
  hashTable->values[2] = 3;
  hashTable->values[3] = 4;
  hashTable->values[4] = 5;

  // Iterate over the hash table and print the values.
  for (int i = 0; i < hashTable->size; i++) {
    printf("%d\n", hashTable->values[i]);
  }
}
