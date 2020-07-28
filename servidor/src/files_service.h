#include "librerias.h"
#define TAM 256
#define BUFFSIZE 256

int32_t listar(int32_t Id_C_Mensajes);
char *obtenerMD5(char *filename);
int32_t cliente2();
void enviarImagen(int32_t sockfd,char *direccionImagen);
int32_t obtenerTamanio(char *direccionImagen);