#include "files_service.h"
#include "colas_mensajes.h"
#include "utilidades.h"
/**	
 * @brief main recibe primer mensaje del primary_server y valida la funcion a ejecutar
*/
int32_t main()
{   
    int32_t Id_C_Mensajes = crearCola();
    recibirMensajeCola(Id_C_Mensajes);
    int32_t status=1;
    do
    {
        if(m.dato_numerico==5 && !strcmp(m.mensaje,"filels"))
        {
           status=listar(Id_C_Mensajes); 
        }
        else if (m.dato_numerico==6)
        {
            status=cliente2(Id_C_Mensajes);
        }
        else    
            status =0;

    }while(status);
}
/**
 * @brief Funcion encargada de leer la carpeta que contien las imagens y listar
 * @param int Id_C_Mensajes
 * @return 0
 * */
int32_t listar(int32_t Id_C_Mensajes)
{
    
    char *imagen=getenv("PWD");
    char *direccion = getenv("PWD");
	char *archivo = "/imagenes";
	strcat(direccion, archivo);
    DIR *d = opendir(direccion);
    struct stat datos;
    struct dirent *dentry;
    int32_t tamanio;
    while((dentry=readdir(d)) != NULL)
    {
        tamanio=0;
        if(strlen(dentry->d_name)>3)
        {
            sprintf(imagen,"imagenes/%s",dentry->d_name);
            if(stat(imagen, &datos)==-1)
                perror("stat");
            else
            { 
                recibirMensajeCola(Id_C_Mensajes);
                tamanio= (int32_t)(datos.st_size/1024)/1024;
                enviarMensajeCola(2,tamanio,dentry->d_name,Id_C_Mensajes); 
                
                //parte para el MD5
                recibirMensajeCola(Id_C_Mensajes);
                tamanio=(int32_t)(datos.st_size/1024)/1024;
                enviarMensajeCola(2,0,obtenerMD5(imagen),Id_C_Mensajes); 
            }
        }
    }
    recibirMensajeCola(Id_C_Mensajes);
    enviarMensajeCola(2,29,"final",Id_C_Mensajes); 
    closedir(d);
    return 0;
}

/**
 * @brief funcion que genera el MD5sum de un archivo
 * @param char*filename : direccion del archi
 * @return char* md5 : cadena con md5sum
 * */
char *obtenerMD5(char *filename)
{
    char *md5=malloc(64*sizeof(char));
    char *aux=malloc(64*sizeof(char));
    // char *aux=NULL;
    unsigned char c[MD5_DIGEST_LENGTH];
    int32_t i;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    size_t bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        free(aux);
        free(md5);
        return 0;
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, sizeof(char), sizeof(data), inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
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
 * @brief crea un socket cliente inet, se conectara con el server2 creado en el cliente
 * y le transmitira la imagen, luego se cierra
 * @param int Id_C_Mensajes
 * */
int32_t cliente2(int32_t Id_C_Mensajes)
{

    recibirMensajeCola(Id_C_Mensajes);
    char *direccionImagen=getenv("PWD");
    char *nombreImagen=malloc(64*sizeof(char));
    //char *nombreImagen=NULL;
    strcpy(nombreImagen,m.mensaje);
    sprintf(direccionImagen,"imagenes/%s",nombreImagen);
    enviarMensajeCola(2,25,"entre",Id_C_Mensajes); 
    free(nombreImagen);
	int32_t sockfd, puerto,status=1;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	puerto = 8080;
	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	
	if ( sockfd < 0 ) 
	{
		perror( "ERROR apertura de socket" );
		exit( 1 );
	}

	server = gethostbyname("127.0.0.1");
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

	while(status)
	{   
        enviarImagen(sockfd,direccionImagen);
        status=0;
	}
    close(sockfd);
    return 0;
}
void enviarImagen(int32_t sockfd,char *direccionImagen)
{
    //aca transferir tamaño
    int32_t tamanio=obtenerTamanio(direccionImagen);
    fprintf(stderr,"envio tamaño de: %i\n",tamanio);
    send(sockfd,&tamanio,sizeof(tamanio),0);
    FILE *imagen = fopen(direccionImagen,"rb");
    char buffer[BUFFSIZE];
    while(!feof(imagen))
    {
		if(fread(buffer,sizeof(char),sizeof(buffer),imagen))
		if(send(sockfd,buffer,sizeof(buffer),0) == -1)
			perror("Error al enviar el arvhivo:");
	}
    fclose(imagen);
}
int32_t obtenerTamanio(char *direccionImagen)
{
    struct stat datos;
    int32_t tamanio=0;
    if(stat(direccionImagen, &datos)==-1)
        perror("stat");
    else
    { 
        tamanio= (int32_t)(datos.st_size); 
    }
    return tamanio;     
}