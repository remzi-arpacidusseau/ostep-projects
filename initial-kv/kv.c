#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Node.h"
#include "database.h"

#define TOK_DELIM ","
#define BUFSIZE 256

void put(int key, char *value) {
  int result = putIntoDB(key, value);
  if (!result)
  {
    printf("Could not put item in db.\n");
    exit(1);
  }
}

void get(int key) {
  int result = getFromDB(key);
  if (!result)
  {
    printf("Could not get item from db.\n");
    exit(1);
  }
}

void all() {
  int result = getAllFromDB();
  if (!result)
  {
    printf("Could not get all items from db.\n");
    exit(1);
  }
}

void clear() {
  int result = clearDB();
  if (!result)
  {
    printf("Could not clear from db.\n");
    exit(1);
  }
}

void delete(int key) {
  int result = deleteFromDB(key);
  if (!result)
  {
    printf("Could not delete item from db.\n");
    exit(1);
  }
}

void parse(char *line) {
  int key;
  char *value;
  char *buffer;
  char *op = strtok(line, ",");
  buffer = strtok(NULL, ",");
  if (buffer)
  {
    key = atoi(buffer);
    buffer = strtok(NULL, ",");
    if (buffer)
    {
      value = buffer;
    }
  }
  switch (*op)
  {
  case 'p':
    put(key, value);
    break;
  case 'g':
    get(key);
    break;
  case 'a':
    all();
    break;
  case 'c':
    clear();
    break;
  case 'd':
    delete(key);
    break;
  default:
    printf("bad command\n");
  }
}

void execute(int argc, char **argv)
{
  int index = 1;
  do
  {
    parse(argv[index]);
    index++;
  } while (index < argc);
}

void setup(int argc)
{
  if (argc == 1)
    exit(0);
  setupDB();
}

int main(int argc, char **argv) {
  setup(argc);
  execute(argc, argv);
  closeDB();
  return EXIT_SUCCESS;
}