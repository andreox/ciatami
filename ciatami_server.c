#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFLEN 256

int main ( int argc , char* argv[] ) {

  int socket_fd , newsocket_fd , client_len , n , port_number ;

	char* buffer = (char*) malloc(sizeof(char)*BUFLEN) ;

	struct sockaddr_in serv_addr , client_addr ;

	if ( argc < 2 ) {

		fprintf( stderr , "Usage : ./ciatami_server <port_number>\n") ;
		return 1 ;

	}

	socket_fd = socket( AF_INET , SOCK_STREAM , 0) ; //Create a new socket
	if ( socket_fd < 0 ) {

		fprintf( stderr , "Error creating socket\n") ;
		return 1 ;

	}

	bzero( (char*) &serv_addr , sizeof(serv_addr)) ; //Initialize to 0 serv_addr
	port_number = atoi(argv[1]) ; //Initialize port number

	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_port = htons(port_number) ; //Converting port number in network byte order
	serv_addr.sin_addr.s_addr = INADDR_ANY ; //Set IP address value

	if ( bind( socket_fd , ( struct sockaddr* ) &serv_addr , sizeof(serv_addr) ) < 0 ) { //Bind socket connection

		fprintf( stderr , "Error on binding\n") ;
		return 1 ;

	}

	listen( socket_fd , 5 ) ; //Listen for client connection. When it founds a client , we will initialize it
	client_len = sizeof(client_addr) ; 
	newsocket_fd = accept( socket_fd , (struct sockaddr* ) &client_addr , &client_len ) ; // Server accept request of connection by client

	if ( newsocket_fd < 0 ) {

		fprintf( stderr , "Error accepting client\n") ;
		return 1 ;

	}

	while( 1 ) {

		bzero( buffer , BUFLEN ) ; //Buffer initializing to 0
		n = read( newsocket_fd , buffer , BUFLEN ) ;

		if ( n < 0 ) {

			fprintf( stderr , "Error reading the socket\n") ;
			return 1 ;

		}

		fprintf( stdout , "Client : %s", buffer) ;
		bzero( buffer , BUFLEN ) ;
		fprintf( stdout , "Insert message to send to the client : ") ;
		fgets( buffer , BUFLEN , stdin ) ;

		n = write( newsocket_fd , buffer , strlen(buffer) ) ;
		if ( n < 0 ) {

			fprintf( stderr , "Error writing to socket\n") ;
			return 1 ;

		}

	}

	return 0 ;

}
