#include "myapue.h"
#include <sys/socket.h>

int s_pipe(int fd[2])
{
    return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd)); 
}
