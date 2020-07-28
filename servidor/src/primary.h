#include "librerias.h"
#include "colas_mensajes.h"
#include "utilidades.h"

#define TAM 256
char nuevaContra[TAM];
int32_t filaContra;

/*Declaro funciones*/

int32_t validarComando(char buffer[TAM],int32_t newsockfd);
int32_t auth_service(int32_t funcion,int32_t newsockfd);
int32_t comunicacionValidar(int32_t Id_C_Mensajes,int32_t newsockfd);
void comunicacionUser(int32_t Id_C_Mensajes,int32_t newsockfd);
void cambiarContrasenia(int32_t Id_C_Mensajes,int32_t newsockfd);
int32_t files_service(int32_t funcion,int32_t newsockfd,char * dirImagen);
void recibirListadoImagenes(int32_t newsockfd,int32_t Id_C_Mensajes);
void enviarImagen(int32_t newsockfd,int32_t Id_C_Mensajes,char * dirImagen);

