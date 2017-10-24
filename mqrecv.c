#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <mqueue.h>

void display_usage(void) {
	printf("usage: mqrecv [options]\n"
           "options:\n"
           "  -n <name>  Message Queue name\n"
           "  -b         Block for a message to receive\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {
	char * name = NULL;
	bool blocking = false;

	for (int c = 0; (c = getopt(argc, argv, "n:bh?")) != -1;) {
		switch (c) {
			case 'n':
				name = optarg;
				break;

			case 'b':
				blocking = true;
				break;

			case 'h':
			case '?':
				display_usage();
				break;

			default:
				abort();
		}
	}

	errno = 0;
	mqd_t queue;
	if (blocking) {
		queue = mq_open(name, O_RDONLY);
	} else {
		queue = mq_open(name, O_RDONLY | O_NONBLOCK);
	}
	if (queue == (mqd_t)-1) {
		if (errno == ENOENT) {
			fprintf(stderr, "No queue with that name exists.\n");
			exit(EXIT_FAILURE);
		} else {
			perror("mq_open");
			exit(EXIT_FAILURE);
		}
	}

	struct mq_attr attr;
	int r = mq_getattr(queue, &attr);
	if (r == -1) {
		perror("getattr");
		return 1;
	}

	long buflen = attr.mq_msgsize + 1;
	char buf[buflen];
	unsigned int msg_prio;
	errno = 0;
	ssize_t len = mq_receive(queue, buf, buflen, &msg_prio);
	if (len == -1) {
		if (errno == EAGAIN) {
			fprintf(stderr, "No message available.\n");
			return 2;
		} else {
			perror("mq_receive");
			return 1;
		}
	}

	printf("%d %s\n", msg_prio, buf);
	r = mq_close(queue);
	if (r == -1) {
		perror("mq_close");
		return 1;
	}
}
