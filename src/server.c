#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int socket_desc;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_desc == -1)
	{
		perror("initializing socket");
		exit(EXIT_FAILURE);
	}

	printf("Hello, World!\n");
	return 0;
}
