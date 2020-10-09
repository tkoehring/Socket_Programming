#pragma once

#include <queue>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include "../structs.h"
#include "../defines.h"

extern "C"
{
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <netinet/in.h>
        #include <netdb.h>
        #include <unistd.h>
        #include <string.h>
}

class server
{
	private:
		int sockfd;
		std::queue<struct job> tasks;
		std::unordered_map<std::string, std::string> files;
		std::thread workers[NUM_THREADS];
		std::mutex mtx;
		//Methods

	public:
		//Methods
		server(){sockfd = -1;}
		~server(){};
		void launch();
		void work(void);

};
