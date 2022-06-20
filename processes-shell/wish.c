#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#define PROMPT "wish>"
#define STR_SIZE 50

int procCommands(char *line);
int procCommand(char *command);
char *trim(char *str);
const char *getCommand(char *command);
char **parseCommand(char *command);
int isBuiltin(char *bin);
int doBuiltin(int no, char **command);
int doCD(char **command);
int setPath(char **command);
int isRedirect(char *command, char *redirectPath);
int isRedirectPathValid(char *filepath);
int isParallel(char *command);
int procParallelCommand(char *command);

char *path[10] = {"/bin"};
char *builtin[10] = {"exit", "cd", "path"};
char error_message[30] = "An error has occurred\n";

int main(int argc, char *argv[]) {
  FILE *fp=NULL;
  if (argc==2) { // parse files batch
    fp = fopen(argv[1], "r");
    // open file failed
    if (fp==NULL) {
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }
  } else if (argc==1){ // interactive mode
    fp = stdin;
    printf("%s ", PROMPT);
  } else { // only accept no arg or 1 arg
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }

  // char *path =  getenv(argv[1]);
  char *line = NULL;
  size_t linecap = 20;

  // prompt loop
  while (getline(&line, &linecap, fp) > 0) {
    // TODO: 1. process commands
    procCommands(line);
    if (argc==1)
      printf("%s ", PROMPT);
  }
  return 0;
}

int procCommands(char *line) {
  char *token, *string, *tofree;
  tofree = string = strdup(trim(line));
  assert(string != NULL);
//  printf("commands start\n");

  if (isParallel(string) == 0) {
    procParallelCommand(string);
  } else {
    while ((token = strsep(&string, "&")) != NULL) {
      if (strcmp(token, "")!=0)
        procCommand(token);
    }
  }

//  printf("commands end\n");
  free(tofree);
  return 0;
}

int procCommand(char *command) {
  char *trimmed = trim(command);
  if (strcmp(trimmed, "")==0) printf("is 0 str\n");
  char **argv = parseCommand(trimmed);
//  printf("%s\n",argv[0]);

  // if it's builtin commands
  int builtinNo;
  if ((builtinNo=isBuiltin(argv[0])) != -1) {
    doBuiltin(builtinNo, argv);
    return 0;
  }

  int rc = fork();
  if (rc==0) {
//    printf("%lu\n", strlen(argv[0]));
    // binary file
    char *commandWithPath = strdup(getCommand(argv[0]));

    // process redirect
    char redirectPath[STR_SIZE]="";
    int isR = isRedirect(trimmed, redirectPath);
    if (isR > 0) {
      close(1);
      if (isR >= 3 || isRedirectPathValid(redirectPath) != 0) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
      }
      int fd = open(redirectPath, O_CREAT | O_WRONLY, 0644);
      dup(fd);
    }

    execv(commandWithPath, argv);
  } else {
    rc = (int) wait(NULL);
  }

  return -1;
}

char *trim(char *str) {
  char *end;
  while(isspace((unsigned char) *str)) str++;
  if (*str==0) return str;
  end=str+strlen(str)-1;
  while(end > str && isspace((unsigned char) *end)) end--;
  end[1] = '\0';
  return str;
}

const char *getCommand(char *command) {
  // if empty command
  // case1: > /tmp/output
  if (strlen(command) == 0) {
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }
  // if empty path, only run builtin commands
  if (path[0]==NULL) {
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }
  int i=0;
  while (path[i]) {
    char *fullCmd = strdup(path[i++]);
    strcat(fullCmd, "/");
    strcat(fullCmd, command);
    if (access(fullCmd, X_OK)==0)
      return fullCmd;
  }
//  perror("perror");
//  printf("access failed\n");
  write(STDERR_FILENO, error_message, strlen(error_message));
  exit(1);
}

char **parseCommand(char *command) {
  char **parsedCommand = malloc(10 * sizeof(char*));
  for (int i =0 ; i < 10; ++i)
    parsedCommand[i] = malloc(STR_SIZE * sizeof(char));
  char *token, *string, *tofree;
  tofree = string = strdup(command);
  assert(string != NULL);
  int i=0;
  while ((token = strsep(&string, " \t")) != NULL) {
    char *trimmed = trim(token);
    if (strcmp(trimmed, "")==0) continue;

    int isR = 0;
    for (int j=0; j < strlen(trimmed); ++j) {
      if (trimmed[j] == '>') {
        isR = 1;
        trimmed[j] = '\0';
        break;
      }
    }
    parsedCommand[i++] = strdup(trimmed);
    if (isR == 1) break;
  }
  parsedCommand[i] = NULL;

  free(tofree);
  return parsedCommand; // TODO: need to be free
}

int doBuiltin(int no, char **command) {
//  printf("do built in: No.%d\n", no);
  switch (no) {
    case 0:
      if (command[1]!=NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(0);
      }
      exit(0);
    case 1:
      doCD(command);
      break;
    case 2:
      setPath(command);
      break;
    default:
      printf("default bug\n");
      exit(1);
  }

  return 0;
}

int isBuiltin(char *bin) {
  for (int i=0; builtin[i]!=NULL; ++i) {
    if (strcmp(bin, builtin[i])==0)
      return i;
  }
  return -1;
}

int doCD(char **command) {
//  printf("doCD\n");
  int len = 0;
  while (command[len]!=NULL) len++;
  if (len!=2) {
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(0);
  }
  const char *arg = strdup(command[1]);
//  printf("%s\n", arg);
  int result = chdir(arg);
  if (result!=0) {
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(0);
  }

  return result;
}

int setPath(char **command) {
  int i=1;
  while (command[i]!=NULL) {
    path[i-1] = strdup(command[i]);
    ++i;
  }
  path[i-1]=NULL;
  return 0;
}

// return number of '>' (1 >, 1 file), -1 if no redirect
int isRedirect(char *command, char *redirectPath) {
  char *token;
  char filename[STR_SIZE];
  int countRe = 0;
  while ((token = strsep(&command, ">")) != NULL) {
    countRe++;
    strcpy(filename, token);
  }

  char *trimmed = trim(filename);
  if (strlen(trimmed) != 0) {
    strcpy(redirectPath, trimmed);
  }

  if(countRe <= 1) {
    return -1;
  } else {
    return countRe;
  }
}

int isRedirectPathValid(char *filepath) {
  // empty filename
  if (strlen(filepath) == 0) {
    return -1;
  }

  // multifiles
  int i=0;
  while (filepath[i] != '\0') {
    if (filepath[i] == ' ') {
      return -1;
    }
    i++;
  }

  return 0;
}

int isParallel(char *command) {
  for (int i=0; i< strlen(command); ++i) {
    if (command[i] == '&') {
      return 0;
    }
  }

  return -1;
}

int procParallelCommand(char *command) {
  // separate parallel commands
  char **commands = malloc(10 * sizeof(char*));
  for (int i =0 ; i < 10; ++i)
    commands[i] = malloc(STR_SIZE * sizeof(char));
  char *token;
  int no = 0;
  while ((token = strsep(&command, "&")) != NULL) {
    strcpy(commands[no], trim(token));
    no++;
  }

  // process commands parallel
  for (int i=0; i<no; ++i) {
    char **argv = parseCommand(commands[i]);
    if (fork() == 0) {
      char *commandWithPath = strdup(getCommand(argv[0]));

      // process redirect
      char redirectPath[STR_SIZE]="";
      int isR = isRedirect(commands[i], redirectPath);
      if (isR > 0) {
        close(1);
        if (isR >= 3 || isRedirectPathValid(redirectPath) != 0) {
          write(STDERR_FILENO, error_message, strlen(error_message));
          exit(1);
        }
        int fd = open(redirectPath, O_CREAT | O_WRONLY, 0644);
        dup(fd);
      }
      execv(commandWithPath, argv);
    }
  }

//  int status = 0;
  // wait for all children processes finish
  while (wait(NULL) > 0) {
//      printf("%d child completed\n", status++);
  }

  return 0;
}
