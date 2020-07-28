#include "primary.h"
/**
 * @brief recibe lo enviado por el cliente, y valida si es un comando
 * del servidor o no, en caso de ser un comando llama a la funcion encargada
 * @param char buffer[TAM] : buffer enviado por el cliente
 * 			   int32_t newsockfd : sockfd del cliente
 * @return 0 si el comando no es valid
 * 		   1 si es un comando valido
 * 		   2 si el comando enviado fue exit
 * */
int32_t validarComando(char buffer[TAM],int32_t newsockfd)
{
	char** informacion=malloc(64*sizeof(char*));
	ParseLn(buffer,informacion);
	if (!strcmp("user",informacion[0]) && !strcmp("ls",informacion[1]))
	{
		auth_service(1,newsockfd);
		return 1;
	}		
	else if (!strcmp("user",informacion[0]) && !strcmp("passwd",informacion[1]) && informacion[2]!=NULL)
	{
		strcpy(nuevaContra,informacion[2]);
		auth_service(2,newsockfd);
		return 1;
	}
	else if (!strcmp("file",informacion[0]) && !strcmp("ls",informacion[1]))
	{
		files_service(0,newsockfd,NULL);
		return 1;
	}
	else if (!strcmp("file",informacion[0]) && !strcmp("down",informacion[1]) && informacion[2]!=NULL)
	{
		files_service(1,newsockfd,informacion[2]);
		return 1;
	}
	else if (!strcmp("exit",informacion[0]))
	{
			return 2;
	}
	else
		return 0;
}
/**	
 * @brief ejecuta el proceso de auth service,
 * funcion, comunica que funcion va a ejecutar el proceso
 * funcion :0 --> validarusuario()
 * funcion :1 --> userls()
 * funcion :2 --> user passwd nueva_passwd
 * @param int32_t funcion: comunica que funcion va a ejecutar el proceso
 * 		  int32_t newsockfd : sockfd del cliente
 * @return linea de la bd que esta el usuario que se conecto

*/
int32_t auth_service(int32_t funcion,int32_t newsockfd)
{
	int32_t lineaU=0;
	int32_t pid = fork();
    if (pid == -1)
    {
        perror( "fork" );
        exit(1);
    }
    else if (pid==0)
    { 
        execv("./bin/auth_service",(char *[]) {NULL});
        perror("Error en ecex");
    }
    else
    {	
		int Id_C_Mensajes = crearCola();
		switch(funcion)
		{	
			case 0://valida usuario
				lineaU=comunicacionValidar(Id_C_Mensajes,newsockfd);
				controlCola(Id_C_Mensajes);
				break;
			case 1://user ls
				comunicacionUser(Id_C_Mensajes,newsockfd);
				controlCola(Id_C_Mensajes);
				break;
			case 2://user ls
				cambiarContrasenia(Id_C_Mensajes,newsockfd);
				controlCola(Id_C_Mensajes);
				break;
		}
        wait(NULL);
    }
	return lineaU;  
}
/**	
 * @brief Funcion que hace la comunicacion con el auth_Service y el cliente
 * para validar el usuario 
 * @param int Id_C_Mensajes id de la cola de mensajes
 * 		  int32_t newsockfd : sockfd del cliente
*/
int32_t comunicacionValidar(int32_t Id_C_Mensajes,int32_t newsockfd)
{
	enviarMensajeCola(2,29,"validar",Id_C_Mensajes);
	char buffer[TAM];
    int32_t status=1;
	int32_t lineaUsuario=0;
    while(status)
    {
		/*parte socket*/
		
		send(newsockfd,"Usuario,Contraseña: ",TAM,0);
		memset( buffer, 0, TAM );
        recv(newsockfd,buffer,TAM-1,0);
		strtok(buffer,"\n"); //le quito el enter guardado en usuario
		
		/*parte auth_Service*/
        enviarMensajeCola(2,29,buffer,Id_C_Mensajes);//envio usuario
        recibirMensajeCola(Id_C_Mensajes);

        if (!strcmp("validado",m.mensaje))
        {
            status = 0;
			lineaUsuario=m.dato_numerico;//guardo la linea del usuario
        }
		else if (!strcmp("incorrecto",m.mensaje))
		{
			controlCola(Id_C_Mensajes);
			send(newsockfd,"exit",TAM,0);
			status=0;
		}
    }
	return lineaUsuario;
}
/** 
 * @brief funcion encargada de recibir la lista de los usuarios de la BD
 *        del lado del cliente tendremos que ir respondiendo al sevidor con un enter
 *        para que este siga listando.
 * @param int Id_C_Mensajes id de la cola de mensajes
 * 		  int32_t newsockfd : sockfd del cliente
 * */
void comunicacionUser(int32_t Id_C_Mensajes,int32_t newsockfd)
{
	
	enviarMensajeCola(2,29,"userls",Id_C_Mensajes);
	char buffer[TAM];
	send(newsockfd,"Usuarios     Activo     Ultima vez",TAM,0);
	int status =1;
	while(status)
    {
		memset( buffer, 0, TAM );
        recv(newsockfd,buffer,TAM-1,0);
		strtok(buffer,"\n"); //le quito el enter guardado en usuario

		/*parte auth_Service*/
        enviarMensajeCola(2,29,buffer,Id_C_Mensajes);//envio usuario
        recibirMensajeCola(Id_C_Mensajes);
		/*si recibo final, es porq ya se listaron todos los usuarios*/
        if (!strcmp("final",m.mensaje))
        {
			controlCola(Id_C_Mensajes);
            status = 0;
        }
		else
		{
			send(newsockfd,m.mensaje,TAM,0);
		}
    }	
}
/** 
 * @brief funcion encargada cambiar la contraseña del usuario en la base de datos
 * @param int Id_C_Mensajes id de la cola de mensajes
 * 		  int32_t newsockfd : sockfd del cliente
 * */
void cambiarContrasenia(int32_t Id_C_Mensajes,int32_t newsockfd)
{
	enviarMensajeCola(2,filaContra,nuevaContra,Id_C_Mensajes);
	send(newsockfd,"Contraseña Cambiada",TAM,0);
	char buffer[TAM];
	memset( buffer, 0, TAM );
    recv(newsockfd,buffer,TAM-1,0);
	strtok(buffer,"\n"); //le quito el enter guardado en usuario

}
/**	
 * @brief ejecuta el proceso de files service,
 * funcion, comunica que funcion va a ejecutar el proceso
 * funcion :0 --> recibirListadoImagenes()
 * funcion :1 --> enviarImagen()
 * @param int32_t funcion: comunica que funcion va a ejecutar el proceso
 * 		  int32_t newsockfd : sockfd del cliente
 *        char * dirImagen :direccion de la imagen que se va a enviar
 * @return 0
*/
int32_t files_service(int32_t funcion,int32_t newsockfd,char * dirImagen)
{
	int32_t pid = fork();
    if (pid == -1)
    {
        perror( "fork" );
        exit(1);
    }
    else if (pid==0)
    { 
        execv("./bin/files_service",(char *[]) {NULL});
        perror("Error en ecex");
    }
    else
    {	
		int Id_C_Mensajes = crearCola();
		switch(funcion)
		{	
			case 0://recibo listado imagenes
				recibirListadoImagenes(newsockfd,Id_C_Mensajes);
				controlCola(Id_C_Mensajes);
				break;
			default://envio imagen
				enviarImagen(newsockfd,Id_C_Mensajes,dirImagen);
				controlCola(Id_C_Mensajes);
				break;
    	}
		wait(NULL);
	}
	return 0;  
}
/**	
 * @brief funcion que se comunica con el files_server para que este cree el socket y mande la imagen
 * funcion, comunica que funcion va a ejecutar el proceso
 * funcion :0 --> recibirListadoImagenes()
 * funcion :1 --> enviarImagen()
 * @param int Id_C_Mensajes id de la cola de mensajes
 * 		  int32_t newsockfd : sockfd del cliente
 *        char * dirImagen :direccion de la imagen que se va a enviar
*/
void enviarImagen(int32_t newsockfd,int32_t Id_C_Mensajes,char * dirImagen)
{	int32_t status=1;
	char buffer[TAM];
	enviarMensajeCola(2,6,dirImagen,Id_C_Mensajes);
	send(newsockfd,"Comenzar(enter)",TAM,0);
	do
	{
		memset( buffer, 0, TAM );
        recv(newsockfd,buffer,TAM-1,0);
		strtok(buffer,"\n"); 
		enviarMensajeCola(2,29,dirImagen,Id_C_Mensajes);//envio usuario
        recibirMensajeCola(Id_C_Mensajes);
		if(!strcmp(m.mensaje,"entre"))
        	status = 0;
    } while (status);
}
/**	
 * @brief funcion que se comunica con el files_server para que este liste las imagens disponibles
 * funcion, comunica que funcion va a ejecutar el proceso
 * funcion :0 --> recibirListadoImagenes()
 * funcion :1 --> enviarImagen()
 * @param int Id_C_Mensajes id de la cola de mensajes
 * 		  int32_t newsockfd : sockfd del cliente
*/
void recibirListadoImagenes(int32_t newsockfd,int32_t Id_C_Mensajes)
{
	int32_t status=1;
	char *linea=malloc(TAM*sizeof(char*));
	char buffer[TAM];
	enviarMensajeCola(2,5,"filels",Id_C_Mensajes);
	send(newsockfd,"Listado : Nombre - Tamaño",TAM,0);
	do
	{	
		memset( buffer, 0, TAM );
        recv(newsockfd,buffer,TAM-1,0);
		strtok(buffer,"\n"); 
		enviarMensajeCola(2,29,buffer,Id_C_Mensajes);//envio usuario
        recibirMensajeCola(Id_C_Mensajes);
        if (!strcmp("final",m.mensaje))
        {
            status = 0;
        }
		else
		{
			if(m.dato_numerico>0)
			sprintf(linea,"%s - %i mb (enter y esperar MD5)",m.mensaje,m.dato_numerico);
			else
			{
				sprintf(linea,"MD5: %s (enter)",m.mensaje);
			}
			send(newsockfd,linea,TAM,0);
		}
	}
	while(status);
}