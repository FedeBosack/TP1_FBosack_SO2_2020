#include "client_utilities.h"

int32_t main( int32_t argc, char *argv[] ) 
{
	int32_t puerto,terminar=0,recibir=0;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[TAM];
	if ( argc < 3 ) 
	{
		fprintf( stderr, "Uso %s host puerto\n", argv[0]);
		exit( 0 );
	}

	puerto =  atoi( argv[2] );
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	
	if ( sockfd < 0 ) 
	{
		perror( "ERROR apertura de socket" );
		exit( 1 );
	}

	server = gethostbyname( argv[1] );
	if (server == NULL) 
	{
		fprintf( stderr,"Error, no existe el host\n" );
		exit( 0 );
	}
	memset( (char *) &serv_addr, '0', sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, (size_t )server->h_length );
	serv_addr.sin_port = htons( puerto );


	if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr ) ) < 0 ) 
	{
		perror( "Error de conexion" );
		exit( 1 );
	}
	//nuevo
	// activar handler para SIGINT
	struct sigaction sa;
	sa.sa_handler = salida;
	sigaction(SIGINT, &sa,  NULL);
	//
	while(1)
	{
		memset( buffer, '\0', TAM );
		recv(sockfd,buffer,TAM,0);
		if(!strcmp("exit",buffer)) 
		{
			printf("Error en validacion de usuario o contraseña\n");
			send(sockfd,"exit",strlen(buffer),0);
			exit(0);
		}
		
		fprintf(stderr, "%s", buffer);
		if(!strcmp("Comenzar(enter)",buffer))
		{
			recibir=1;
		}

		memset( buffer, 0, TAM );
		if(fgets( buffer, TAM-1, stdin ))
		
		if(buffer[0] == '\n' )
		{
			strcpy(buffer,"ningun comando\n");
		}

		send(sockfd,buffer,strlen(buffer),0);
		buffer[strlen(buffer)-1] = '\0';

		if(!strcmp( "exit", buffer ) ) 
		{
			terminar = 1;
		}

		if(recibir)
		{
			servidor2();
			recibir=0;
		}


		if( terminar ) 
		{
			printf( "Finalizando ejecución\n" );
			exit(0);
		}

	}
	return 0;
}
