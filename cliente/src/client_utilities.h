#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <errno.h>
#include <inttypes.h>
#include <byteswap.h>

#define BUFFSIZE 256
#define TAM 256

#define DEFAULT_HOST "127.0.0.1"
int32_t sockfd;
//para la parte de tabla MBR
#define CANT_PARTICIONES 4
struct mbr              //Estructura (16bytes) para la tabla MBR. 
{
  uint8_t boot;         // Indica si es "booteable"
  uint16_t start_chs;   // Comienzo de CHS (no se utiliza en el tp)
  uint8_t start_chs2;
  uint8_t type;         // Tipo de partición
  uint16_t end_chs;     // Final de CHS (no se utiliza en el tp)
  uint8_t end_chs2;
  //uint32_t start;       // Sector de arranque de la partición   
  //uint32_t size;       // Tamaño de la partición (medido en sectores)
  char start[4];       // Sector de arranque de la partición   
  char size[4];       // Tamaño de la partición (medido en sectores)
} __attribute__((__packed__));

void analisisMbr(char *direccion_usb);
void little_to_big(char big[8], char little[4]);
//funciones
void servidor2();
void recibirArchivo(int32_t newsockfd);
char *obtenerMD5(char *filename,int32_t tamanio);
void salida(int32_t);


