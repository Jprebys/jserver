#include <stdio.h>
#include <sys/socket.h>

#include "utils.h"

int main()
{
	int socket_desc;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_desc == -1)
		error_and_die("Creating socket");

	printf("Hello, World!\n");
	return 0;
}
