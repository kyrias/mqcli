#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

void display_usage(void) {
	printf("usage: mqsend [options]\n"
           "options:\n"
           "  -n <name>     Message Queue name\n"
           "  -m <message>  Message to send to queue\n"
           "  -p <prio>     Priority to send message at [default: 0]\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
	char * name = NULL;
	char * message = NULL;
	unsigned int msg_prio = 0;

	for (int c = 0; (c = getopt(argc, argv, "n:m:p:h?")) != -1;) {
		switch (c) {
			case 'n':
				name = optarg;
				break;

			case 'm':
				message = optarg;
				break;

			case 'p':
				msg_prio = strtoul(optarg, NULL, 10);
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

	if (!message) {
		fprintf(stderr, "Missing message to send.\n");
		exit(EXIT_FAILURE);
	}

	mqd_t queue = mq_open(name, O_WRONLY, 0644, 0);
	if (queue == (mqd_t)-1) {
		if (errno == ENOENT) {
			fprintf(stderr, "No queue with that name exists.\n");
			exit(EXIT_FAILURE);
		} else {
			perror("mq_open");
			exit(EXIT_FAILURE);
		}
	}

	int r = mq_send(queue, message, strlen(message), msg_prio);
	if (r != 0) {
		perror("mq_send");
		exit(EXIT_FAILURE);
	}

	r = mq_close(queue);
	if (r == -1) {
		perror("mq_close");
		exit(EXIT_FAILURE);
	}
}
