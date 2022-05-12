#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if(argc < 2) {
		return 0;
	}

	for(int i = 1; i < argc; i++) {
		FILE* fp = fopen(argv[i], "r");

		if(fp == NULL) {
			printf("wcat: cannot open file\n");
			exit(1);
		}

		for(char ch = fgetc(fp); !feof(fp); ch = fgetc(fp)) {
			fputc(ch, stdout);	
		}
	}

	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
