typedef struct Node
{
  int key;
  char *value;
  struct Node *next;
} Node;

void putIntoLL(int key, char *value, Node **head);
void getFromLL(int key, Node *head);
void getAllFromLL(Node *head);
void clearLL(Node **head);
void deleteFromLL(int key, Node *head);
void insert_end(Node** node, int key, char* value);
void deallocate(Node**);
void removeNode(Node** root, int key);