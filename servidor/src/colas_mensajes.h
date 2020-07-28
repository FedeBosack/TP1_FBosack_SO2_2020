#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <netdb.h> 
#include <unistd.h>

#define TAM 256

int32_t crearCola();
void enviarMensajeCola(long id_mensaje,int32_t dato_numerico,char buffer[TAM],int32_t Id_C_Mensajes);
void recibirMensajeCola(int32_t Id_C_Mensajes);
void comunicacion(int32_t Id_C_Mensajes);
void controlCola(int32_t Id_C_Mensajes);

struct S_Mensaje
{
		long id_mensaje;
		int32_t dato_numerico;
		char mensaje[TAM];
}m;