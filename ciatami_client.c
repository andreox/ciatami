#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFLEN 256

int main ( int argc , char* argv[] ) {

  int socket_fd , port_number , n ;
	struct sockaddr_in serv_addr ;
	struct hostent *server ;

	char buffer[BUFLEN] ;

	if (argc < 3) {

       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       return 1 ;

    }

    port_number = atoi(argv[2]);
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
    	fprintf(stderr , "Error opening the socket\n") ;
    	return 1 ;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

	bzero( (char*) &serv_addr , sizeof(serv_addr)) ; //Initialize to 0 serv_addr
	serv_addr.sin_family = AF_INET ;
	bcopy( (char*) server->h_addr , (char*) &serv_addr.sin_addr.s_addr , server->h_length ) ; // Copy hostent address to serv_addr
	serv_addr.sin_port = htons(port_number) ;

	if ( connect( socket_fd , (struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0 ) {

		fprintf(stderr , "Failed connection\n") ;
		return 1 ;

	}

	while( 1 ) {

		fprintf( stdout , "Enter message to send to the client : ") ;
		bzero( buffer , BUFLEN ) ;
		fgets( buffer , BUFLEN-1 , stdin ) ;
		n = write( socket_fd , buffer , strlen(buffer) ) ;
		if ( n < 0 ) {

			fprintf(stderr , "Error to send message to the server\n") ;
			return 1 ;

		}

		bzero( buffer , BUFLEN ) ;
		n = read( socket_fd , buffer , BUFLEN ) ;
		if ( n < 0 ) {

			fprintf( stderr , "Error to read message from the server\n") ;
			return 1 ;

		}

		fprintf( stdout , " Server : %s\n" , buffer ) ;

	}
	close( socket_fd ) ;
	return 0 ;

}
