#include "utilidades.h"
/**
 * @brief Funcion que parsea la linea de entrada para verificar el usuario, contraseña,
 * bloqueado o no y ultima conexion
 * @param char *line : linea a ser parseada
 *        char* informacion[] : puntero de puntero recibido donde se parseara la linea
 * */
void ParseLn(char *line,char* informacion[])
{
   unsigned int posicion = 0,bufsize=64;

    char *parte=NULL;
    parte = strtok(line,DELIM);//primera palabra
    
    while (parte != NULL)
    {   
        informacion[posicion] = parte;
        if (posicion >= bufsize)
        {
    	    informacion = realloc(informacion,bufsize*sizeof(char*));
        }
        parte = strtok(NULL,DELIM);
        posicion++;
    }
}
/**
 * @brief Funcion encargada de modificar la bd y bloquear el usuario
 * @param int linea: linea de fila del usuario a eliminar
 *        char* direccion: direccion de la bd          
 *        char** informacion[] : puntero de puntero recibido donde se parseara la linea
 * */
void bloquearUsuario(int32_t linea,char *direccion,char** informacion)
{
	char *mensaje=malloc(64*sizeof(char*));
	sprintf(mensaje,"%s,%s,%s,%s\n", informacion[0], informacion[1],"false",informacion[3]);
	escribirEnArchivo(linea,mensaje,direccion,1);
}
/**
 * @brief Funcion encargada de escribir una linea en el archivo
 * @param int linea
 *        char *mensaje
 *        char *direccion
 *        int32_t flag
 * */
void escribirEnArchivo(int32_t linea,char *mensaje,char *direccion,int32_t flag)
{
    FILE *fd;
    size_t bufsize = 0; //tamaño del buffer de entrada 
	char* line=NULL;//contenera cada linea del archivo
    char **auxiliar = malloc(TAM*sizeof(char*));
    int32_t nlinea = 0;
    fd = fopen(direccion,"r+");
   
    while(!feof(fd))
    {
        if(nlinea==linea)
        {
            if(getline(&line, &bufsize, fd))//obtengo la linea pero no la guardo
            auxiliar[nlinea]=mensaje;
        }
        else
        {
            if(getline(&line, &bufsize, fd))
            auxiliar[nlinea]=line;
        }
        line=NULL;
        nlinea ++;
        
    }
    fclose(fd);
    /* Aca escribo el archivo con los nuevos valores*/
    fd = fopen(direccion,"wt");
    for(int32_t i =0;i<nlinea-flag;i++)
	
    fprintf(fd,"%s",auxiliar[i]);
    fclose(fd);
    free(auxiliar);//nuevo
}
/**
 * @brief Funcion encargada de escribir una linea en el archivo con la ultima conexion
 * @param int line
 *        char *direccion
 *        int32_t flag
 * */
void guardarConexion(int32_t linea, char *direccion,char** informacion)
{
	char *mensaje=malloc(64*sizeof(char));
    sprintf(mensaje,"%s,%s,%s,%s\n", informacion[0], informacion[1], informacion[2],ultimaConexion());
	escribirEnArchivo(linea,mensaje,direccion,1);
    free(mensaje);
}
/**
 * @brief funcion que retorna un char* con el horario actual y dia
 * @return conexion
 * */
char *ultimaConexion()
{
    char *conexion=malloc(64*sizeof(char));
    time_t tiempo = time(NULL);
    struct tm *tlocal = localtime(&tiempo);
    int32_t dia,mes,anio,hora,minuto;
    
    hora = tlocal->tm_hour;
    minuto=tlocal->tm_min;
    dia=tlocal->tm_mday;
    mes=tlocal->tm_mon+1;
    anio=1900+tlocal->tm_year;
    sprintf(conexion, "H:%d:%d_D:%d/%d/%d", hora,minuto,dia,mes,anio);
    return conexion;
}
/**
 * @brief Funcion encargada de cambiar la contraseña
 * @param int line
 *        char *direccion
 *        char** informacion
 *        char *contra
 * */
void escribirContrasenia(int32_t linea, char *direccion,char** informacion,char *contra)
{
	char *mensaje=malloc(64*sizeof(char*));;
    sprintf(mensaje,"%s,%s,%s,%s", informacion[0], contra, informacion[2],informacion[3]);
	escribirEnArchivo(linea,mensaje,direccion,1);
}