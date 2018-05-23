#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "data.h"

#define SERWER_PORT 8888
#define SERWER_IP "127.0.0.1"



namespace Network{

	class Client{
	public:
		Client();
		data send(char* message, unsigned int usec=100);
		void close();
	private:
		int m_activity;
		struct sockaddr_in m_server;
		char buffer[ 4096 ] = { };
		char m_message[ 4096 ] = { };
		int m_socket;
		fd_set master;   // główna lista deskryptorów plików
		fd_set read_fds; // pomocnicza lista deskryptorów dla select()

	};


}
#endif
