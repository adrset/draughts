#include "ListenerServer.h"
namespace network{
ListenerServer::ListenerServer(){
  serwer =
    {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
    };
    serwer.sin_addr.s_addr = INADDR_ANY;
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
	
    if( inet_pton( AF_INET, SERWER_IP, & serwer.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
   
    socket_ = socket( AF_INET, SOCK_DGRAM, 0 );
    if(( socket_ ) < 0 )
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }
   
   
    len = sizeof( serwer );
    if( bind( socket_,( struct sockaddr * ) & serwer, len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
	
}

void ListenerServer::close(){

 shutdown( socket_, SHUT_RDWR );
}

Data ListenerServer::listen()
{
	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
	
  
     	FD_ZERO(&read_fds);
  
        //add master socket to set
        FD_SET(socket_, &read_fds);
        client = { };
       
        memset( buffer, 0, sizeof( buffer ) );
       

	activity = select( socket_ + 1, &read_fds , NULL , NULL , &read_timeout);

	if ((activity < 0)) 
        {
            printf("select error");
        }

     	if (FD_ISSET(socket_, &read_fds)) {
		if( recvfrom( socket_, buffer, sizeof( buffer ), 0,( struct sockaddr * ) & client, & len ) < 0 )
		{
		    perror( "recvfrom() ERROR" );
		    exit( 4 );
		}
		//printf( "|Message from client|: %s \n", buffer );
		strcpy(resp, buffer);
	       
		char buffer_ip[ 128 ] = { };
		//printf( "|Client ip: %s port: %d|\n", inet_ntop( AF_INET, & client.sin_addr, buffer_ip, sizeof( buffer_ip ) ), ntohs( client.sin_port ) );
	       
		strncpy( buffer, "1337 ACK", sizeof( buffer ) );
	       
		if( sendto( socket_, buffer, strlen( buffer ), 0,( struct sockaddr * ) & client, len ) < 0 )
		{
		    perror( "sendto() ERROR" );
		    exit( 5 );
		}
		return Data(false, resp, strlen( resp ));
    
	}
	return Data();
   
}
}
