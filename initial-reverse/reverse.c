#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct linkedList
{
  char *line;
  struct linkedList *next;
} LinkedList;

int main(int argc, char *argv[])
{
  if (argc > 3)
  {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }
  FILE *in = stdin;
  FILE *out = stdout;
  if (argc == 2 && ((in = fopen(argv[1], "r")) == NULL))
  {
    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
    exit(1);
  }
  if (argc == 3)
  {
    if ((in = fopen(argv[1], "r")) == NULL)
    {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      exit(1);
    }
    if ((out = fopen(argv[2], "w")) == NULL)
    {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
      exit(1);
    }
    struct stat sb1, sb2;
    if (fstat(fileno(in), &sb1) == -1 || fstat(fileno(out), &sb2) == -1)
    {
      fprintf(stderr, "reverse: fstat error\n");
      exit(1);
    }
    if (sb1.st_ino == sb2.st_ino)
    {
      fprintf(stderr, "reverse: input and output file must differ\n");
      exit(1);
    }
  }

  LinkedList *head = NULL;
  size_t linecap = 0;
  char *line = NULL;

  while (getline(&line, &linecap, in) != -1)
  {
    LinkedList *node = malloc(sizeof(LinkedList));
    if ((node->line = strdup(line)) == NULL)
    {
      fprintf(stderr, "reverse: strdup failed\n");
    }
    node->next = head;
    head = node;
  }
  
  while (head != NULL)
  {
    LinkedList *temp = head;
    fprintf(out, "%s", head->line);
    head = head->next;
    free(temp->line);
    free(temp);
  }
  free(line);
  fclose(in);
  fclose(out);
  return 0;
}