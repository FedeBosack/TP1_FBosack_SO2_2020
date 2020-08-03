#include "client_utilities.h"
/**
 * @brief crea un socket server inet, solo ejecutara la funcion de
 * recibir archivo, luego se cierra
 * */
void servidor2()
{
    int32_t newsockfd ,sockfd;
	uint32_t clilen;
	
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) 
	{ 
		perror( " apertura de socket ");
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	//puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8080);
	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) 
	{
		perror( "ligadura" );
		exit( 1 );
	}

    printf( "Socket disponible: %d\n", ntohs(serv_addr.sin_port) );

	listen( sockfd, 1 );
	clilen = sizeof(cli_addr);

	newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
	if ( newsockfd < 0 ) 
	{
		perror( "accept" );
		exit( 1 );
	}
	close(sockfd);
	int32_t status=1;
	while (status) 
	{	
		recibirArchivo(newsockfd);
		status =0;
	}
	close(newsockfd);		
}
/**
 * @brief funcion que genera el MD5sum de un archivo
 * @param char*filename : direccion del archi
 * @return char* md5 : cadena con md5sum
 * */
char *obtenerMD5(char *filename,int32_t tamanio)
{
	sync();
    char *md5=malloc(MD5_DIGEST_LENGTH * 2 + 1);
    char *aux=malloc(64*sizeof(char));
    // char *aux=NULL;
    unsigned char c[MD5_DIGEST_LENGTH];
    int32_t i;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    size_t bytes;
	ssize_t total=0;
    unsigned char data[1024];

    if (inFile == NULL) 
	{
        printf ("%s can't be opened.\n", filename);
        free(aux);
        free(md5);
        return 0;
    }

    MD5_Init (&mdContext);

    while (total<(ssize_t)tamanio)
	{
		bytes = fread (data, sizeof(char), sizeof(data), inFile);
        MD5_Update (&mdContext, data, bytes);
    	total =total+(ssize_t)bytes;
	}
	MD5_Final (c,&mdContext);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) 
    {
        sprintf(aux,"%02x", c[i]);
        strcat(md5,aux);
    }
    free(aux);
    fclose (inFile);
    
    return md5;
}
/**
 * Handler de para terminar la conexion del cliente
 * se llama cuando se presiona Ctrl + C
 */
void salida(int32_t sig) 
{
	if(sig>0) 
	{
		printf("Aprete Ctrl-C , Chau!\n");
		send(sockfd,"exit\n",strlen("exit\n"),0);
	}
	exit(0);
}
/**
 * @brief recibe un archivo y lo escribe en la direccion deseada
 * @param int32_t newsockfd : sockfd del cliente que envia el archio
 * */

void recibirArchivo(int32_t newsockfd)
{
	FILE *file;
	char buffer[BUFFSIZE];
	ssize_t recibido = -1;
	int32_t tamanio=0;
	//int32_t i=0;
	if(recv(newsockfd, &tamanio,sizeof(tamanio),0)>0)
	{
		fprintf(stderr,"recibi el tamaño de : %i\n",tamanio);
		/*Se abre el archivo para escritura*/
		file = fopen("/dev/sdb","wb");
		if(file != NULL)
		{
			//recibir tamaño del archivo
			while((recibido = recv(newsockfd, buffer,sizeof(buffer), 0)) > 0)
			{
				//if(recv(newsockfd, buffer,sizeof(buffer), 0)){};
				fwrite(buffer,sizeof(char),sizeof(buffer),file);
				//fprintf(stderr,".");
				//i++;
			}
			fprintf(stderr,"Termino la descarga!\n");
			fflush(stdout);
			fclose(file);
			fprintf(stderr,"Calculando Hash MD5 (esperar)\n");
			sync(); // NUEVO
			char *md5sum=(char*)obtenerMD5("/dev/sdb",tamanio);
			fprintf(stderr,"MD5: %s\n",md5sum);
			free(md5sum);
			fprintf(stderr,"Calculando Tabla MBR (esperar)\n");
			sync(); // NUEVO
			analisisMbr("/dev/sdb");
		}
		else
		{
			perror("lectura usb");
			exit(1);
		}
	}
}
//NUEVO - PARTE TABLA DE PARTICIONES MBR
/**
 * @brief Funcion que ejecuta el analisis mbr, para un maximo de 4 particiones
 * 
 * @param direccion_usb 
 */
void analisisMbr(char *direccion_usb)
{
	printf("Partición\tBooteable\tTipo\tInicio\tTamaño\n");
	for(int32_t iParticion = 0; iParticion < CANT_PARTICIONES; iParticion++) 
	{

		struct mbr tabla_mbr;
		FILE *usb = fopen(direccion_usb, "rb");
		if(usb != NULL)
		{
			fseek(usb, 0L, SEEK_SET);
			fseek(usb, 446 + 16 * iParticion, SEEK_CUR);//por cada particion (16 bytes)

			if(fread(&tabla_mbr, sizeof(tabla_mbr), 1, usb) > 0)
			fclose(usb);
		}
		else
		{
			perror("lectura usb");
			exit(1);
		}

		char boot[3], tipo[3];
		
		sprintf(tipo, "%02x", tabla_mbr.type & 0xff);
		sprintf(boot, "%02x", tabla_mbr.boot & 0xff);
		//compruebo si es o no booteable
		if(!strcmp(boot,"80"))
			sprintf(boot,"SI");
		else if( strcmp(boot, "00") == 0 )
			sprintf(boot,"NO");
		else
			sprintf(boot,"ER");
		
		
		printf("    %i\t\t%s\t\t%s\t%d\t%d Mb\n",
				iParticion+1,boot,tipo,tabla_mbr.start,tabla_mbr.size*512/1000000);
	}
}
