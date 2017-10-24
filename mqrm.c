#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>

void display_usage(void) {
	printf("usage: mqrm [options]\n"
           "options:\n"
           "  -n <name>  Message Queue name\n"
           "  -f         Ignore errors\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
	char * name = NULL;
	bool force = false;

	for (int c = 0; (c = getopt(argc, argv, "n:fh?")) != -1;) {
		switch (c) {
			case 'n':
				name = optarg;
				break;

			case 'f':
				force = true;
				break;

			case 'h':
			case '?':
				display_usage();
				break;

			default:
				abort();
		}
	}

	if (!name) {
		fprintf(stderr, "Missing Message Queue name.\n");
		exit(EXIT_FAILURE);
	}

	int r = mq_unlink(name);
	if (r == -1 && !force) {
		if (errno == ENOENT) {
			fprintf(stderr, "No queue with that name exists.\n");
			exit(EXIT_FAILURE);
		} else {
			perror("mq_unlink");
			exit(EXIT_FAILURE);
		}
	}
}
