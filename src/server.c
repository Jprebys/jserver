#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include "utils.h"

#define MAX_CONNS 10
#define BUF_LEN 1024

int main()
{
	int listen_socket;

	printf("\n\n");

    printf("Configuring local address...\n");
    struct addrinfo hints = {
    	.ai_family = AF_INET,
    	.ai_socktype = SOCK_STREAM,
    	.ai_flags = AI_PASSIVE
    };
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    print_addrinfo(bind_address, "bind address");

    // create socket
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1)
		error_and_die("creating socket");

	// bind socket
	if (bind(listen_socket, bind_address->ai_addr, bind_address->ai_addrlen))
		error_and_die("binding socket");

	// listen socket
	if (listen(listen_socket, MAX_CONNS) == -10)
		error_and_die("listening socket");

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listen_socket, &master);

	// need to track max fd for select
	int max_socket = listen_socket;

	printf("Waiting for connections...\n");

	while (1)
	{
		fd_set reads;
		reads = master;
		if (select(max_socket+1, &reads, 0, 0, 0) == -1)
			error_and_die("Select");

		for (int i = 0; i <= max_socket; ++i)
		{
			if (FD_ISSET(i, &reads))
			{
				if (i == listen_socket)
				{
					// in this case our listen socket is 
					// telling us there are new connections available
					// so we need to call accept on them
					struct sockaddr_storage client_addr;
					socklen_t client_len = sizeof(client_addr);
					int socket_client = accept(listen_socket, (struct sockaddr *) &client_addr, &client_len);
					if (socket_client == -1)
						error_and_die("accepting new connection");
					FD_SET(socket_client, &master);
					if (socket_client > max_socket)
						max_socket = socket_client;

					char address_buffer[100];
					getnameinfo((struct sockaddr *) &client_addr, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
					printf("New connection from %s\n", address_buffer);

				}
				else
				{
					// in this case, an existing connection is ready to be read
					char read[BUF_LEN];
					int bytes_received = recv(i, read, BUF_LEN, 0);
					if (bytes_received < 1)
					{
						// the client closed the connection
						printf("Client disconnected...\n");
						FD_CLR(i, &master);
						close(i);
						continue;
					}
					printf("%.*s", bytes_received, read);

				}




			}  // if FD_ISSET
		}  // for i <= max_socket
	}  // while 1



	printf("\n\n");
	return 0;
}
