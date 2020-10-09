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

class client
{
	private:
		int sockfd;
		std::unordered_map<std::string, std::string> files;
		std::thread workers[NUM_THREADS];
		std::mutex m;
		//Methods
	public:
		//Methods
		client(){sockfd = -1;}
		~client(){};
		void launch();
		void work(void);

};
