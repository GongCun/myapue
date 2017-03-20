#include "myapue.h"
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define DELIMITER "%\t\n"
#define MAXARGC 2

static void
free_exit(void);
char           *str;

int
main(int argc, char *argv[])
{
	FILE           *fp;
	int             nread, i, k = 0;
	int             tty;
	char            buf[MAXLINE], *ptr, *cmdmap[MAXARGC], readin[MAXLINE];
	char            ch;


	if (argc != 2) {
		fprintf(stderr, "usage: %s <cmd-file>\n", argv[0]);
		return -1;
	}
	if (atexit(free_exit) != 0)
		err_sys("atexit");

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}
	if ((tty = open("/dev/tty", O_RDWR)) < 0)
		err_sys("open /dev/tty");

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (strtok(buf, DELIMITER) == NULL) {
			fprintf(stderr, "format is not right\n");
			return -1;
		}
		cmdmap[i = 0] = buf;
		while ((ptr = strtok(NULL, DELIMITER)) != NULL) {
			if (++i > MAXARGC - 1) {
				fprintf(stderr, "format is not right\n");
				return -1;
			}
			cmdmap[i] = ptr;
		}

		while ((nread = read(STDIN_FILENO, &ch, 1)) == 1) {
			if (k >= (int) sizeof(readin) - 1)
				k = 0;
			readin[k++] = ch;
			readin[k] = 0;
			if (ch == '\n') {
				if (write(tty, readin, strlen(readin)) != (ssize_t) strlen(readin))
					err_sys("write tty");
				k = 0;
			}
			if (strcmp(readin, cmdmap[0]) == 0 || strregex(cmdmap[0], readin) == 1) {
				if (ch != '\n')
					if (write(tty, readin, strlen(readin)) != (ssize_t) strlen(readin))
						err_sys("write tty");
				if (!str)
					str = malloc(strlen(cmdmap[1]) + 2);
				else
					str = realloc(str, strlen(cmdmap[1]) + 2);
				strcpy(str, cmdmap[1]);
				strcat(str, "\n");

				if (write(STDOUT_FILENO, str, strlen(str)) != (ssize_t) strlen(str))
					err_sys("write stdout");
				k = 0;
				break;
			}
		}
		if (nread < 0)
			err_sys("read stdin error");
	}

	if (write(tty, "\n", 1) != 1)
		err_sys("write tty");

	if (ferror(fp))
		err_sys("read fp error");

	fclose(fp);
	exit(0);
}

static void
free_exit(void)
{
	if (str)
		free(str);
}
