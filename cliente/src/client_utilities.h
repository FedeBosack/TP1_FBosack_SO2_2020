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

#define BUFFSIZE 256
#define TAM 256
#define DEFAULT_HOST "127.0.0.1"

//funciones
void servidor2();
void recibirArchivo(int32_t newsockfd);
char *obtenerMD5(char *filename,int32_t tamanio);
