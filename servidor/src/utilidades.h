#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>


#define TAM 256
#define DELIM ",\t "

void ParseLn(char *line,char* informacion[]);
void bloquearUsuario(int32_t linea,char *direccion,char** informacion);
void escribirEnArchivo(int32_t linea,char *mensaje,char *direccion,int32_t flag);
void guardarConexion(int32_t linea,char *direccion,char** informacion);
char *ultimaConexion();
void escribirContrasenia(int32_t linea, char *direccion,char** informacion,char *contra);



