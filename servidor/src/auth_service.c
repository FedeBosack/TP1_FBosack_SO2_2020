#include "auth_service.h"
#include "colas_mensajes.h"
#include "utilidades.h"

/**	
 * @brief main recibe primer mensaje del primary_server y valida la funcion a ejecutar
*/
int32_t main()
{
	int32_t Id_C_Mensajes = crearCola();
	recibirMensajeCola(Id_C_Mensajes);

    int32_t status = 1;
    do
    {
        if (!strcmp("validar",m.mensaje))
        {
			status=validarUsuario(Id_C_Mensajes);
        }
        else if (!strcmp("userls",m.mensaje))
        {
			status=listarUsuarios(Id_C_Mensajes);
        }
		else
		{
			status = cambiarContrasenia();
		}
		
    }while(status);
}
/**
 * @brief Funcion encargada de leer la base de datos de usuarios, y validar en caso que esten bien el 
 * usuario y contraseña, o en caso de escribir tres veces mal la contraseña de un usuario, bloquearlo
 * En el caso de escribir tres veces mal el usuario, es cliente se cerrara
 * @param int Id_C_Mensajes
 * @return 0
 * */
int32_t validarUsuario(int32_t Id_C_Mensajes)
{
	char *direccion = getenv("PWD");
	char *archivo = "/usuarios.csv";
	strcat(direccion, archivo);
	char** informacion=malloc(64*sizeof(char*));
	char** datosUsuario=malloc(64*sizeof(char*));
	//para archivo
	size_t bufsize = 0; //tamaño del buffer de entrada
	char* line=NULL;//contenera cada linea del archivo
	int32_t j = 0,linea;
	do
	{	
		j++;
		recibirMensajeCola(Id_C_Mensajes);
		linea=0;
		ParseLn(m.mensaje,datosUsuario);
		FILE *fd;
		fd = fopen(direccion,"r");
		while(!feof(fd))
		{
			if(getline(&line, &bufsize, fd))
			strtok(line,"\n");
			/*parseo linea por linea y obtengo cantidad de argumentos*/
			ParseLn(line,informacion);
			line=NULL;
			/* verifico el usuario */
			if(!strcmp(datosUsuario[0],informacion[0]))
			{
				if(!strcmp(datosUsuario[1],informacion[1]) && !strcmp(informacion[2],"true"))
				{
					guardarConexion(linea,direccion,informacion);
					enviarMensajeCola(2,linea,"validado",Id_C_Mensajes);
					return 0;
				}
				else if(j==3)
				{
					//fprintf(stderr,"Se bloqueo el usuario: %s \n",informacion[0]);
					bloquearUsuario(linea,direccion,informacion);
					enviarMensajeCola(2,29,"incorrecto",Id_C_Mensajes);
					return 0;
				}
			}
			linea++;
		}
		fclose(fd);
		if(j==3)
		{
			enviarMensajeCola(2,29,"incorrecto",Id_C_Mensajes);
			return 0;
		}
		else
			enviarMensajeCola(2,29,"devuelta",Id_C_Mensajes);
		//j++;	
	}while(j<3);
	return 0;
}
/**
 * @brief Funcion encargada de leer la base de datos de usuarios,y listarlos
 * @param int Id_C_Mensajes
 * @return 0
 * */
int32_t listarUsuarios(int32_t Id_C_Mensajes)
{
    size_t bufsize = 0;
	char* line=NULL;
	char buffer[TAM];
	char *direccion = getenv("PWD");
	char *archivo = "/usuarios.csv";
	strcat(direccion, archivo);
    //char *direccion = "/home/fedebos/Escritorio/TP1/servidor/usuarios.csv";
    char **informacion = malloc(TAM*sizeof(char*));
	int32_t cantidad =0,i=0;
	FILE *fd;
    fd = fopen(direccion,"rt");
    while(!feof(fd))
	{
		if(getline(&line, &bufsize, fd))
		line=NULL;
        cantidad ++;
    }
    fclose(fd);
    fd = fopen(direccion,"rt");
    while(!feof(fd))
	{
		recibirMensajeCola(Id_C_Mensajes);
		if(getline(&line, &bufsize, fd))
		ParseLn(line,informacion);
        if(i<cantidad-1)
        {
			//memset( buffer, 0, TAM );
			sprintf(buffer,"%s     %s     %s",informacion[0],informacion[2],informacion[3]);
			buffer[strlen(buffer)-1] = '\0';
			enviarMensajeCola(2,29,buffer,Id_C_Mensajes);
        }
		else
		{
			enviarMensajeCola(2,29,"final",Id_C_Mensajes);
		}
        i++;
		line=NULL;
    }
	fclose(fd);
	return 0;
}
/**
 * @brief Funcion encargada de leer la base de datos de usuarios, y cambiar la contraseña
 * 		  del usuario actual
 * @return 0
 * */
int32_t cambiarContrasenia()
{
	int32_t status=1;
	size_t bufsize = 0;
	char* line=NULL;
	char *direccion = getenv("PWD");
	char *archivo = "/usuarios.csv";
	strcat(direccion, archivo);
	char **informacion = malloc(TAM*sizeof(char*));
	int32_t fila =0;
	FILE *fd;
	fd = fopen(direccion,"rt");
	do
	{
		while(!feof(fd))
		{
			if(getline(&line, &bufsize, fd))
			ParseLn(line,informacion);
			if(fila==m.dato_numerico)//si estoy en la fila correcta
			{
				escribirContrasenia(fila,direccion,informacion,m.mensaje);
			}
			fila++;
			line=NULL;
		}
		fclose(fd);
		status =0;
	} while (status);
	
	return 0;
}

