#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define errpf(string, ...) fprintf(stderr, string, __VA_ARGS__)

int print_help(char** argv) {
	printf(
		"File2Bin - a command line utility to convert any file to C code\n"
		"Args: %s [args]\n"
		" -h - Display this message\n"
		" -f - Specify input file\n"
		" -o - Specify output file, otherwise will write to stdout\n"
		"WARNING: THIS WILL OVERWRITE YOUR FILE!!!!!!!!!!!!!\n"
		" -v - Specify the string variable name\n"
		"\n"
		"Forged in the depths of hell by blek\n\n",
		argv[0]
	);
	return 0;
}

void rand_str(char *dest, size_t length) {
	srand(time(0));
	char charset[] = "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (length-- > 0) {
		size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
		*dest++ = charset[index];
	}
	*dest = '\0';
}

int main(int argc, char** argv) {

	char c = 0;
	const char* file;
	unsigned char file_provided = 0;

	const char* varname = calloc(sizeof(char), 8*sizeof(char));
	rand_str(varname, 8);

	FILE* outf = stdout;

	while ((c = getopt(argc, argv, "hf:v:o:")) != -1) {
		switch (c) {
			case 'h':
				print_help(argv);
				return 0;
			case 'f':
				file = calloc(sizeof(char), (strlen(optarg) + 1) * sizeof(char));
				memcpy(file, optarg, strlen(optarg) + 1);
				file_provided = 1;
				break;
			case 'v':
				free(varname);
				varname = calloc(sizeof(char), (strlen(optarg) + 1) * sizeof(char));
				memcpy(varname, optarg, strlen(optarg) + 1);
				break;
			case 'o':
				outf = fopen(optarg, "w+");
				if (!outf) {
					errpf("Cannot open file %s!", optarg);
				}
				break;
		}
	}

	if (!file_provided) {
		printf("File must be provided.\n");
		exit(-1);
	}

	FILE* f = fopen(file, "rt");
	if (!f) {
		errpf("Can't open source file \"%s\"!\n", file);
		exit(-1);
	}

	fseek(f, 0, SEEK_END);
	size_t src__len = ftell(f);
	fseek(f, 0, SEEK_SET);

	const char* text = calloc(sizeof(char), src__len);
	fread(text, 1, src__len, f);
	
	if (!text) {
		errpf("Can't open source file \"%s\"!\n", file);
	}
//	printf("---- START OF SOURCE FILE TEXT ----\n%s\n---- END OF SOURCE FILE TEXT ----\n", text);

	char* hex_array = malloc(2);
	memcpy(hex_array, "\"", 2);
	char* tmp = malloc(1);
	size_t tmp_sz = 0;

	for (size_t i = 0; i != src__len; i++) {
		free(tmp);
		
		tmp_sz = snprintf(NULL, 0, "%s\\x%02X", hex_array, text[i]);
		tmp = calloc(sizeof(char), tmp_sz);

		sprintf(tmp, "%s\\x%02X", hex_array, text[i]);
		free(hex_array);
		hex_array = calloc(sizeof(char), strlen(tmp) + 1);
		memcpy(hex_array, tmp, strlen(tmp) + 1);
	}

	fprintf(outf, "const char %s[] = %s\";\n", varname, hex_array);
	if (outf != stdout) fclose(outf);

}
