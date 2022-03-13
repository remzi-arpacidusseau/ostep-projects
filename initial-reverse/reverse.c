#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


typedef struct Node {
    char *val;
    struct Node *next;
} Node;


int main(int argc, char *argv[]) {
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }
    
    FILE *fin = (argc < 2) ? stdin : fopen(argv[1], "r");
    if (fin == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
        return 1;
    }
    FILE *fout = (argc < 3) ? stdout : fopen(argv[2], "w");
    if (fout == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
        return 1;
    }

    struct stat sin, sout;
    if (fstat(fileno(fin), &sin) == -1) {
        fprintf(stderr, "fstat error\n");
        return 1;
    }
    if (fstat(fileno(fout), &sout) == -1) {
        fprintf(stderr, "fstat error\n");
        return 1;
    }
    if (sin.st_dev == sout.st_dev && sin.st_ino == sout.st_ino) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        return 1;
    }
    
    Node *head = NULL;
    Node *node;
    char *line = NULL;
    size_t len;
    while (getline(&line, &len, fin) != -1) {
        node = malloc(sizeof(Node));
        if (node == NULL) {
            fprintf(stderr, "malloc failed\n");
            return 1;
        }
        node->val = malloc(len);
        if (node->val == NULL) {
            fprintf(stderr, "malloc failed\n");
            return 1;
        }
        for (int i = 0; i < len; ++i) node->val[i] = line[i];
        node->next = head;
        head = node;
    }

    while (head != NULL) {
        fprintf(fout, "%s", head->val);
        head = head->next;
    }
}
