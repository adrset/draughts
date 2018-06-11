#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "Data.h"




namespace network{

	class Client{
	public:
		Client(const char*, int port);
		Data send(const char* message, unsigned int usec=100);
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
