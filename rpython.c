#include <stdio.h>
#include <stdlib.h>

#include "Python.h"

int main(int argc, char const *argv[]) {
	long filesize;
	char* source;
	int lend, pos1, pos2;
	int line;

	if (argc <= 1) {
		fprintf(stderr, "No argument was given, one was expected.\n");
		return 1;
	}
	if (argc > 2) {
		fprintf(stderr, "%d arguments was given, only one was expected.\n", argc-1);
		return 1;	
	}

	FILE* f = fopen(argv[1], "rb");
	if (f == NULL) {
		fprintf(stderr, "Cannot open file %s\n", argv[1]);
		return 3;
	}

	/* calculate the size of the file */
	fseek(f, 0, SEEK_END);
	filesize = ftell(f);
	fseek(f, 0, SEEK_SET);

	/* load the whole source file into memory */
	source = (char*)malloc(filesize + 1);
	if (source == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return 100;
	}
	fread(source, filesize, 1, f);
	fclose(f);

	source[filesize] = 0;

	/* move trailing spaces to the beginning of the line */
	pos1 = pos2 = 0;
	lend = 0;
	line = 1;
	while (lend < filesize) {
		if (source[lend] == ' ') {
			fprintf(stderr, "Leading spaces are not allowed in .pyr files; one found on line %d.\n", line);
			return EXIT_FAILURE;
		}

		while (source[lend] != '\n' && source[lend] != 0) {
			lend++;
		}
		pos1 = pos2 = lend -1;
		while (source[pos1] == ' ') {
			pos1--;
		}
		while (pos1 < pos2) {
			if (source[pos1] == '\n' || pos1 < 0) {
				source[pos2--] = ' ';
			} else {
				source[pos2--] = source[pos1--];
			}
		}
		lend++;
		line++;
	}

	/* Run the interpreter */
	Py_Initialize();
	PyRun_SimpleString(source);

	free(source);
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }

	return 0;
}
