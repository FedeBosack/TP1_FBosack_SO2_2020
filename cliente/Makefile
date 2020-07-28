# Directorios
CLIENT_SOURCE=src
BINARY_DIR=bin
#Binarios
CLIENT=client
# Opt de compilacion 
CC=gcc
CFLAGS=-std=gnu11  -Wall -Werror -pedantic -Wextra -O1 -Wconversion -g
all: client docs

client :
	mkdir -p $(BINARY_DIR)
	cppcheck ./
	$(CC) $(CFLAGS) $(CLIENT_SOURCE)/$(CLIENT).c $(CLIENT_SOURCE)/client_utilities.c -o $(BINARY_DIR)/$(CLIENT) -lssl -lcrypto
clean:
	rm  -Rf $(BINARY_DIR) html latex
docs:
	doxygen Doxyfile