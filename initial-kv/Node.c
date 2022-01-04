#include <stdlib.h>
#include <stdio.h>
#include "Node.h"

void insert_end(Node** root, int key, char* value) {
  Node* new_node = malloc(sizeof(Node));
  if (new_node == NULL)
  {
    exit(1);
  }
  new_node->key = key;
  new_node->value = value;
  new_node->next = NULL;

  if (*root == NULL) {
    *root = new_node;
    return;
  }
  Node* curr = *root;
  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = new_node;
}

void insert_beginning(Node** root, int key, char* value) {
  Node* new_node = malloc(sizeof(Node));
  if (new_node == NULL) {
    exit(1);
  }
  new_node->key = key;
  new_node->value = value;
  new_node->next = *root;

  *root = new_node;
}

void insert_after(Node* node, int key, char* value) {
  Node *new_node = malloc(sizeof(Node));
  if (new_node == NULL)
  {
    exit(1);
  }
  new_node->key = key;
  new_node->value = value;
  new_node->next = node->next;
  node->next = new_node;
}

void deallocate(Node **root) 
{
  Node *curr = *root;
  while (curr != NULL)
  {
    Node *tmp = curr;
    curr = curr->next;
    free(tmp);
  }
  *root = NULL;
}

void removeNode(Node **root, int key) {
  if (*root == NULL) {
    return;
  } else {
    if ((*root)->key == key)
    {
      Node *node_to_remove = (*root);
      (*root) = (*root)->next;
      free(node_to_remove);
      return;
    }
    else
    {
      for (Node *curr = *root; curr->next != NULL; curr = curr->next)
      {
        if (curr->next->key == key)
        {
          Node *node_to_remove = curr->next;
          curr->next = curr->next->next;
          free(node_to_remove);
          return;
        }
      }
      printf("%d not found", key);
    }
  }
  
}

// int main(int argc, char** argv) {
//   Node* root = NULL;
 
//   insert_end(&root, 1, "ale");
//   insert_end(&root, 2, "mari");
//   insert_end(&root, 3, "sam");
//   Node* curr = root;
//   while (curr != NULL) {
//     printf("%d  %s\n", curr->key, curr->value);
//     curr = curr->next;
//   }
//   deallocate(&root);
//   return 0;
// }

void putIntoLL(int key, char *value, Node **root)
{
  if (*root == NULL)
  {
    insert_beginning(root, key, value);
  }
  else
  {
    Node *curr = *root;
    while (curr != NULL)
    {
      if (curr->key == key)
      {
        curr->value = value;
        return;
      }
      curr = curr->next;
    }
    insert_end(root, key, value);
  }
}

void getFromLL(int key, Node *list)
{
  Node *node = list;
  if (node->key == key)
  {
    printf("%d,%s\n", node->key, node->value);
  }
  else
  {
    while (node != NULL)
    {
      if (node->key == key)
      {
        printf("%s\n", node->value);
        return;
      }
      node = node->next;
    }
  }
}

void getAllFromLL(Node *list)
{
  Node *node = list;
  while (node != NULL)
  {
    printf("%d %s\n", node->key, node->value);
    node = node->next;
  }
}

void clearLL(Node **list)
{
  deallocate(list);
}

void deleteFromLL(int key, Node *list)
{
  Node *node = list;
  if (node->key == key)
  {
    node = node->next;
  }
  while (node->next != NULL)
  {
    if (node->next->key == key)
    {
      node->next = node->next->next;
      return;
    }
    node = node->next;
  }
}