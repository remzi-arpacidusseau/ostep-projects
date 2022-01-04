#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Node.h"
#include "database.h"

/** 
 * This single global variable represents the 
 * data-structure that holds data in-memory 
 */
Node *head;

void setupDB()
{
  FILE *fp = fopen("database.txt", "r");
  if (!fp)
  {
    printf("Could not open db.\n");
    exit(1);
  }
  while (feof(fp) < 1)
  {
    int key;
    char *buffer = malloc((256 + 1) * sizeof(char));
    int lines = fscanf(fp, "%d,%s\n", &key, buffer);
    if (lines > 0)
    {
      insert_end(&head, key, buffer);
    }
  }
  fclose(fp);
}

void closeDB()
{
  FILE *fp = fopen("database.txt", "w");
  if (!fp)
  {
    printf("Could not open db.\n");
    exit(1);
  }

  while (head != NULL) {
    char *buffer = malloc((256+1)*sizeof(char));
    sprintf(buffer, "%d,%s\n", head->key, head->value);
    fwrite(buffer, sizeof(char), strlen(buffer), fp);
    head = head->next;
    free(buffer);
  }

  deallocate(&head);
}

int getFromDB(int key) {
  getFromLL(key, head);
  return 1;
}

int putIntoDB(int key, char *value) {
  putIntoLL(key, value, &head);
  return 1;
}

int getAllFromDB() {
  getAllFromLL(head);
  return 1;
}

int deleteFromDB(int key) {
  removeNode(&head, key);
  return 1;
}

int clearDB() {
 clearLL(&head);
 return 1;
}
