#include "server.h"


void server::work(void)
{
	//printf("In thread..\n");
	struct job j;
	char buffer[SOCK_BUFF_SIZE];
	//printf("Address of q: %p\n", &tasks);
	j.fd = -1;
	while(true)
	{
		mtx.lock();
		if(!tasks.empty())
		{
			printf("Task found..\n");
			j = tasks.front();
			tasks.pop();
			mtx.unlock();
			printf("Job Details:\n\tFD:%d\n\tMessage Type: %d\n\tMessage: %s\n", j.fd, j.m.type, j.m.message);
			if(j.fd != -1)
			{
				if(j.m.type == 1)
				{
					printf("%s\n", j.m.message);
					memset(buffer, 0, SOCK_BUFF_SIZE);
					strcpy(buffer, "world\n");
					printf("Sending message: %s\n", buffer);
					send(j.fd, &buffer[0], SOCK_BUFF_SIZE, 0);
					close(j.fd);
				}
			}
		}
		else
		{
			mtx.unlock();
		}
	}
}

void server::launch()
{
	printf("Starting server..\n");
        int newfd, rc;
        char buffer[SOCK_BUFF_SIZE];
        struct addrinfo hints, *res;
        socklen_t addrlen;
        struct sockaddr_storage clt;
	struct job j;
	struct message m;
	
	for(int i = 0; i < NUM_THREADS; i++)
	{
		workers[i] = std::thread(&server::work, this);
	}

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        //Listen on host port
        rc = getaddrinfo(NULL, PORT, &hints, &res);

        if (rc != 0)
        {
                printf("Could not get address information!\n");
                return;
        }

        //populate socket file descriptor
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
        {
                printf("Could not create socket!\n");
                return;
        }

        //Bind to socket
        if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
        {
                printf("Could not bind socked!");
                close(sockfd);
                return;
        }

			//Listen on the socket for incoming connections
			if (listen(sockfd, 10) == -1)
			{
				printf("Could not listen to socket!");
				close(sockfd);
				return;
			}
	printf("Starting main loop..\n");
        while(true)
        {

		memset(&clt, 0, sizeof(clt));
                //Accept first waiting connection off of queue
                newfd = accept(sockfd, (struct sockaddr *) &clt, &addrlen);

                if (newfd < 0)
                {
			fprintf(stderr, "accept error: %d\n", errno);
                        //printf("Could not accept client!\n");
                        close(sockfd);
                        return;
                }

                memset(buffer, 0, SOCK_BUFF_SIZE);

                //Receive message from new connection into buffer
                rc = recv(newfd, &buffer[0], SOCK_BUFF_SIZE, 0);
                if (rc == 0)
                {
			fprintf(stderr, "Receive Error: %d\n", errno);
                        printf("Connection closed at receive!\n");
                        return;
                }

                if (rc < 0)
                {
                        //printf("Could not receive package!\n");
                        return;
                }
		m.type = buffer[0];
		strcpy(m.message, &buffer[1]);
		printf("Received Message: %s\n", m.message);

                j.fd = newfd;
		j.m = m;
		printf("Address of j.m: %p\n", &j.m);
		printf("Address of m: %p\n", &m);
		mtx.lock();
		tasks.push(j);
		mtx.unlock();
                //close(newfd);
        }



}

