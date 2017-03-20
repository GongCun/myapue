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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#define DELIMITER "%\t\n"
#define MAXARGC 2
#define TIMEOUT 5

static void
free_exit(void);
char           *str;

int
main(int argc, char *argv[])
{
	FILE           *fp, *fptty;
	int             nread, i, k = 0, ret, maxfd;
	int             tty;
	char            buf[MAXLINE], *ptr, *cmdmap[MAXARGC], readin[MAXLINE];
	char            ch;
    struct timeval tv;
    fd_set readfds, writefds;

    maxfd = STDIN_FILENO > STDOUT_FILENO ? STDIN_FILENO : STDOUT_FILENO;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;



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
    if ((fptty = fdopen(tty, "w")) == NULL)
        err_sys("fdopen tty");
    if (setvbuf(fptty, NULL, _IONBF, 0) == EOF)
        err_sys("setvbuf fptty");

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

        for (;;) {
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            FD_ZERO(&writefds);
            FD_SET(STDOUT_FILENO, &writefds);

            ret = select(maxfd + 1, &readfds, &writefds, NULL, &tv);
            if (ret < 0)
                err_sys("select error");
            else if (!ret) {
                fprintf(fptty, "%d seconds elapsed\n", TIMEOUT);
                exit(1);
            }

            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                nread = read(STDIN_FILENO, &ch, 1);
                if (nread < 0) 
                    err_sys("read STDIN_FILENO");
                else if (nread == 0)
                    break;

                if (write(tty, &ch, 1) != 1)
                    err_sys("write tty");
			    readin[k++] = ch;
			    if (k >= (int) sizeof(readin) - 1 || ch == '\n')
				    k = 0;
            }

            if (FD_ISSET(STDOUT_FILENO, &writefds)) {
			    if (strcmp(readin, cmdmap[0]) == 0 || strregex(cmdmap[0], readin) == 1) {
                    strcat(cmdmap[1], "\n");
                    if (write(STDOUT_FILENO, cmdmap[1], strlen(cmdmap[1])) !=
                            (ssize_t)strlen(cmdmap[1]))
                        err_sys("write STDOUT_FILENO");
                    k = 0;
                    break;
                }
            }
        }

        if (nread == 0)
            break;
	}

	if (write(tty, "\n", 1) != 1)
		err_sys("write tty");

	if (ferror(fp))
		err_sys("read fp error");

	fclose(fp);
	fclose(fptty);
	exit(0);
}

static void
free_exit(void)
{
	if (str)
		free(str);
}
