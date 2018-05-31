#ifndef LISTENER_SERVER_H_
#define LISTENER_SERVER_H_

#define SERWER_PORT 8888
#define SERWER_IP "192.168.1.88"
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "Data.h"
namespace network{

	
	class ListenerServer{
	public:	
		ListenerServer();
		Data listen();
		void close();

	private:
		fd_set master;   // główna lista deskryptorów plików
		fd_set read_fds; // pomocnicza lista deskryptorów dla select()
		struct sockaddr_in serwer;
		int socket_;
		int activity;
		char buffer[ 4096 ] = { };
		char resp[ 4096 ] = { };
		socklen_t len;
		struct sockaddr_in client;
	};


}

#endif
