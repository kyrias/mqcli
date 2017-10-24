#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>

void display_usage(void) {
	printf("usage: mqmk [options]\n"
           "options:\n"
           "  -n <name>  Message queue name\n"
           "  -e         Fail if queue already exists\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
	char * name = NULL;
	bool exclusive = false;

	for (int c = 0; (c = getopt(argc, argv, "n:eh?")) != -1;) {
		switch (c) {
			case 'n':
				name = optarg;
				break;

			case 'e':
				exclusive = true;
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

	int oflag = O_CREAT;
	if (exclusive) {
		oflag |= O_EXCL;
	}

	mqd_t queue = mq_open(name, oflag, 0644, NULL);
	if (queue == (mqd_t)-1) {
		if (errno == EEXIST && exclusive) {
			fprintf(stderr, "A queue with that name already exists.\n");
			exit(EXIT_FAILURE);
		} else {
			perror("mq_open");
			exit(EXIT_FAILURE);
		}
	}

	int r = mq_close(queue);
	if (r == -1) {
		perror("mq_close");
		exit(EXIT_FAILURE);
	}
}
