#include "client.h"


void client::launch()
{
        struct addrinfo hints, *res;
	struct message m;
	int rc;
	char buffer[SOCK_BUFF_SIZE];

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = 0;
        hints.ai_protocol = 0;

        //Listen on host port 11155
        rc = getaddrinfo(serverIP, PORT, &hints, &res);

        if (rc != 0)
        {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
		return;
        }

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (sockfd < 0)
        {
                printf("Could not create socket!\n");
		return;
        }

        rc = connect(sockfd, res->ai_addr, res->ai_addrlen);

        if (rc < 0)
        {
                printf("Could not connect to server!\n");
		return;
        }
	m = {};
	m.type = 1;
	strcpy(m.message,"hello");
	printf("Size of M: %d\n", sizeof(m));
	buffer[0] = m.type;
        memcpy(&buffer[1], &m.message, sizeof(m.message));
	printf("Message: %s\n", m.message);
	printf("Buffer Contents: %s\n", &buffer[1]);

        rc = send(sockfd, &buffer[0], SOCK_BUFF_SIZE, 0);

        if (rc < 0)
        {
                printf("Could not send package!\n");
        }

        memset(buffer, 0, SOCK_BUFF_SIZE);
        rc = recv(sockfd, &buffer[0], SOCK_BUFF_SIZE, 0);

        if (rc == 0)
        {
                printf("Connection closed at receive!\n");
		return;
        }

        if (rc < 0)
        {
                printf("Could not receive package!\n");
		return;
        }

        printf("Received Message: %s\n", buffer);
        freeaddrinfo(res);
        close(sockfd);

}



