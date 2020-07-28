# Directorios
SERVER_SOURCE=src
BINARY_DIR=bin
#Binarios
SERVER=primary_server
SERVER_FS=files_service
SERVER_AUTH=auth_service
# Opt de compilacion 
CC=gcc
#CFLAGS=-std=gnu11  -Wall -Werror -pedantic -Wextra -O1 -Wconversion-g
CFLAGS=-std=gnu11  -Wall -Werror -pedantic -Wextra -O1 -Wconversion -g
#--enable=warning,performance,portability,style --language=c
all: server docs
server :
	mkdir -p $(BINARY_DIR)
	cppcheck --enable=warning,performance,portability,style --language=c ./ 
	$(CC) $(CFLAGS) $(SERVER_SOURCE)/primary_server.c $(SERVER_SOURCE)/colas_mensajes.c $(SERVER_SOURCE)/utilidades.c $(SERVER_SOURCE)/primary.c -o $(BINARY_DIR)/$(SERVER)
	$(CC) $(CFLAGS) $(SERVER_SOURCE)/auth_service.c $(SERVER_SOURCE)/colas_mensajes.c $(SERVER_SOURCE)/utilidades.c -o $(BINARY_DIR)/$(SERVER_AUTH)
	$(CC) $(CFLAGS) $(SERVER_SOURCE)/files_service.c $(SERVER_SOURCE)/colas_mensajes.c $(SERVER_SOURCE)/utilidades.c -o $(BINARY_DIR)/$(SERVER_FS) -lssl -lcrypto
.PHONY: clean
clean:
	rm  -Rf $(BINARY_DIR) html latex
docs:	
	doxygen Doxyfile