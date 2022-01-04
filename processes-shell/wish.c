#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFSIZE 256
#define TOK_DELIM " "

void loop(FILE *fp);
void handle_error();
char *trim_line(char *line, char *buffer);
char *read_line(FILE *fp);
char **tokenize(char *line);
void launch(char **args, FILE *fp);
int execute(char **tokens, FILE *fp);
int get_search_path(char path[], char *firstArg);
FILE *setup(int argc, char **argv);

char *search_path[BUFSIZE] = {"/bin", NULL};

int main(int argc, char **argv)
{
  FILE *fp = setup(argc, argv);
  loop(fp);
  return 0;
}

FILE *setup(int argc, char **argv)
{
  if (argc > 2)
  {
    printf("error: too many arguments.\n");
    exit(1);
  }
  FILE *fp = stdin;
  if (argc == 2)
  {
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
      printf("error: could not read from file.\n");
      exit(1);
    }
  }
  return fp;
}

void loop(FILE *fp)
{
  int status = 1;
  char *line;
  char **tokens;

  do
  {
    if (fp == stdin)
    {
      printf("wish> ");
    }
    line = read_line(fp);
    tokens = tokenize(line);
    status = execute(tokens, fp);
    free(line);
    free(tokens);
  } while (status);
}

char *read_line(FILE *fp)
{
  char *line = NULL;
  size_t linecap = 0;
  if (getline(&line, &linecap, fp) == -1)
  {
    if (feof(fp))
    {
      exit(0);
    }
    else
    {
      printf("error reading lines.\n");
      exit(1);
    }
  }
  /* @todo
   * char buffer[BUFSIZE];
   * trim_line(line, buffer);
   */
  line[strlen(line) - 1] = '\0';
  return line;
}

char **tokenize(char *line)
{
  char **tokens = malloc(BUFSIZE * sizeof(char *));
  char *token;
  int position = 0, bufsize = BUFSIZE;
  if (!tokens)
  {
    fprintf(stderr, "error: allocation error.\n");
    exit(1);
  }

  // parse by whitespace
  token = strsep(&line, TOK_DELIM);
  while (token != NULL)
  {
    tokens[position++] = token;
    if (position >= bufsize)
    {
      bufsize += BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens)
      {
        fprintf(stderr, "error: allocation error.\n");
        exit(1);
      }
    }
    token = strsep(&line, TOK_DELIM);
  }
  tokens[position] = NULL;

  /**
   * check for redirection
   * correct syntax is [0 or more args] > [1 arg] NULL
   */
  position = 0;
  if (tokens[0][0] == '>')
    handle_error();
  while (tokens[position] != NULL) {
    if (tokens[position][0] == '>' && tokens[position + 1] == NULL)
      handle_error();

    if (tokens[position][0] == '>' && tokens[position + 1] != NULL && tokens[position + 2] != NULL)
      handle_error();
    position++;
  }

  return tokens;
}

int execute(char **args, FILE *fp)
{
  if (args[0] == NULL)
    handle_error();

  int length = 0;
  while (args[length] != NULL)
    length++;

  if (strcmp(args[0], "exit") == 0)
  {
    if (length > 1)
      handle_error();
    else
    {
      return 0;
    }
  }
  else if (strcmp(args[0], "cd") == 0)
  {
    if (length != 2)
      handle_error();
    else
    {
      if (chdir(args[1]) != 0)
      {
        handle_error();
      }
    }
  }
  else if (strcmp(args[0], "path") == 0)
  {
    int i;
    search_path[0] = NULL;
    for (i = 0; i < length - 1; i++)
      search_path[i] = strdup(args[i + 1]);
    search_path[i + 1] = NULL;
  }
  else
  {
    launch(args, fp);
  }

  return 1;
}

void launch(char **args, FILE *fp)
{
  char path[BUFSIZE];
  if (get_search_path(path, args[0]) == 0)
  {
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
      int stream_type;
      if ((stream_type = fileno(fp)) == -1) {
        handle_error();
        return;
      }
       
      if (stream_type != STDOUT_FILENO)
      {
        if (dup2(stream_type, STDOUT_FILENO) == -1) {
          handle_error();
          return;
        }
          
        if (dup2(stream_type, STDERR_FILENO) == -1) {
          handle_error();
          return;
        }
        fclose(fp);
      }

      if (execv(path, args) == -1)
        handle_error();
    }
    else if (pid < 0)
      handle_error();
    else
      wait(NULL);
  }
  else
  {
    handle_error();
  }
}

int get_search_path(char path[], char *firstArg)
{
  int i = 0;
  while (search_path[i] != NULL)
  {
    snprintf(path, BUFSIZE, "%s/%s", search_path[i], firstArg);
    if (access(path, X_OK) == 0)
      return 0;
    i++;
  }
  return -1;
}

// char *trim_line(char *input_line, char *buffer)
// {
//   int endIdx;
//   for (endIdx = strlen(input_line) - 1; endIdx >= 0; endIdx--)
//     if (input_line[endIdx] != ' ' && input_line[endIdx] != '\t' && input_line[endIdx] != '\n')
//       break;

//   int startIdx = 0;
//   while (input_line[startIdx] == ' ') {
//     startIdx++;
//   }
//   buffer = input_line+startIdx;
//   buffer[endIdx] = '\0';
//   printf("result: |%s|", buffer);
//   return buffer;
// }

void handle_error()
{
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}