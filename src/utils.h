#ifndef _JP_UTILS_H
#define _JP_UTILS_H

#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void error_and_die(char *msg)
{
	printf("Error ");
	perror(msg);
	exit(EXIT_FAILURE);
}

void print_addrinfo(struct addrinfo *info, char *desc)
{
	printf("Info about struct addrinfo (%s):\n", desc);
	while (info != NULL)
	{
		if (info->ai_addr->sa_family == AF_INET)
		{
			struct sockaddr_in *temp = (struct sockaddr_in *) info->ai_addr;
			printf("\tIPv4 addr: %s\n", inet_ntoa(temp->sin_addr));
		}
		else if (info->ai_addr->sa_family == AF_INET6)
		{
			printf("\tIPv6 addr: %s\n", info->ai_addr->sa_data);
		}
		info = info->ai_next;
	}
}


#endif  // _JP_UTILS_H