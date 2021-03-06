#include "Client.h"
namespace network{

Client::Client(const char* ip, int port){

    char message[20] = "";
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    
    std::cout<<"Connecting "<< ip <<":"<<port<<std::endl;

    m_server =
    {
        .sin_family = AF_INET,
        .sin_port = htons( port )
    };

    if( inet_pton( AF_INET, ip, & m_server.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
   
    m_socket = socket( AF_INET, SOCK_DGRAM, 0 );
    if( m_socket < 0 )
    {
        perror( "socket() ERROR" );
        exit( 1 );
    }


}

Data Client::send(const char* message, unsigned int usec)
{
   
    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = usec;

    strcpy(buffer, message);

    FD_SET(m_socket, &read_fds);
    socklen_t len = sizeof( m_server );
    int c = 0;
    if( c = sendto( m_socket, buffer, strlen( buffer ), 0,( struct sockaddr * ) & m_server, len ) < 0 )
    {
        perror( "sendto() ERROR" );
        exit( 1 );
    }

	if(c>0)
	 perror("DIDNT SEND ALL!");
   
    struct sockaddr_in from = { };

    m_activity = select( m_socket + 1, &read_fds , NULL , NULL , &read_timeout);

	if ((m_activity < 0)) 
        {
            printf("select error");
        }
    if (FD_ISSET(m_socket, &read_fds)) {
	    memset( buffer, 0, sizeof( buffer ) );
	    if( recvfrom( m_socket, buffer, sizeof( buffer ), 0,( struct sockaddr * ) & from, & len ) < 0 )
	    {
		perror( "recvfrom() ERROR" );
		exit( 1 );
	    }
	    //printf( "|Server's reply|: %s \n", buffer );
	    strcpy(m_message, buffer);
	    return Data(false, m_message, strlen(m_message));	
     }
     return Data();
   
}

void Client::close(){

 shutdown( m_socket, SHUT_RDWR );

}

}

