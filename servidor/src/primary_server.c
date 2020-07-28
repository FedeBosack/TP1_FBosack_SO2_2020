#include "primary.h"

int32_t main(int32_t argc, char *argv[] ) 
{
	int32_t newsockfd ,sockfd,puerto;
	uint32_t clilen;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;

	if ( argc < 2 ) 
	{
        fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}
	/* fd > 0 --> creacion exitosa */
	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) 
	{ 
		perror( " apertura de socket ");
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) 
	{
		perror( "ligadura" );
		exit( 1 );
	}

    printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );

	listen( sockfd, 1 );
	clilen = sizeof(cli_addr);

	newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
	if ( newsockfd < 0 ) 
	{
		perror( "accept" );
		exit( 1 );
	}
	filaContra=auth_service(0,newsockfd);
	fprintf(stderr,"SERVIDOR: Cliente conectado\n");
	close(sockfd);
			
	while ( 1 ) 
	{
		send(newsockfd,"Comando:",TAM,0);
		memset( buffer, 0, TAM );
		recv(newsockfd,buffer,TAM-1,0);

		if( !strcmp( "exit", buffer ) ) 
		{
			printf( "PROCESO %d. Validacion incorrecta, termino la ejecución.\n\n", getpid() );
			exit(0);
		}
		else
		{
			int32_t r;
			printf( "PROCESO %d. ", getpid() );
			printf( "Recibí: %s", buffer );
			buffer[strlen(buffer)-1] = '\0';
			r = validarComando(buffer,newsockfd);
			if(r==0)
			{
				send(newsockfd,"Respuesta: Comando incorrecto (enter)",TAM,0);
				memset( buffer, 0, TAM );
				recv(newsockfd,buffer,TAM-1,0);
			}
			if(r==2)
			{  	
				close(newsockfd);
				printf( "PROCESO %d. Como recibí 'exit', termino la ejecución.\n\n", getpid() );
				exit(1);
			}
		}
	}			
	return 0; 
}
