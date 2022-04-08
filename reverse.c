#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]) {
	long filesize;
	char* source;
	char buffer[100];
	int pos1, pos2;
	size_t flength;

	if (argc <= 1) {
		fprintf(stderr, "No argument was given, one was expected.\n");
		return 1;
	}
	if (argc > 2) {
		fprintf(stderr, "%d arguments was given, only one was expected.\n", argc);
		return 1;	
	}

	flength = strlen(argv[1]);
	if (flength >= 98) {
		/* we need to have enough space for the new extension */
		fprintf(stderr, "Filename %s is too long\n", argv[1]);
		return 2;
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

	/* move leading spaces to end of the line */
	pos1 = pos2 = 0;
	while(pos1 < filesize) {
		while(source[pos2] == ' ') {
			pos2++;
		}
		while (source[pos1] != '\n' && source[pos1] != 0) {
			if (source[pos2] == '\n' || source[pos2] == 0) {
				source[pos1++] = ' ';
			} else {
				source[pos1++] = source[pos2++];
			}
		}
		pos1++;
		pos2++;
	}

	/* build the new filename */
	strncpy(buffer, argv[1], flength);
	buffer[flength] = 'r';
	buffer[flength+1] = 0;

	f = fopen(buffer, "wb");
	if (f == NULL) {
		fprintf(stderr, "Cannot open file %s for writing\n", buffer);
		return 101;
	}
	fwrite(source, filesize, 1, f);
	free(source);
	fclose(f);

	return 0;
}
