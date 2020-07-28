#include "colas_mensajes.h"
/**
 * @brief crea cola de mensaje
 * @return Id de la cola de mensajes creada
 * */
int32_t crearCola()
{
    key_t Clave1;
	int32_t Id_C_Mensajes;

    Clave1 = ftok ("/bin/ls", 33);
	if (Clave1 == (key_t)-1)
	{
	    fprintf(stderr,"Error al obtener clave para cola mensajes");
		exit(-1);
	}

    Id_C_Mensajes = msgget (Clave1, 0600 | IPC_CREAT);
	if (Id_C_Mensajes == -1)
	{
		fprintf(stderr,"Error al obtener identificador para cola mensajes");
		exit (-1);
	}
    return Id_C_Mensajes;
}
/**
 * @brief envia mensaje pasado por parametro a la cola de mensjajes del ID
 * @param long id_mensaje
 * 		  int dato_numerico
 *        char buffer[TAM]
 *        int Id_C_Mensajes
 * */
void enviarMensajeCola(long id_mensaje,int32_t dato_numerico,char buffer[TAM],int32_t Id_C_Mensajes)
{
    m.id_mensaje = id_mensaje;
	m.dato_numerico = dato_numerico;
	sprintf(m.mensaje,"%s",buffer);

	msgsnd (Id_C_Mensajes, (struct msgbuf *)&m, 
			sizeof(m.dato_numerico)+sizeof(m.mensaje), 
			IPC_NOWAIT);
}
/**
 * @brief recibe el primer mensaje en el buffer de la cola
 * @param int Id_C_Mensajes
 * */
void recibirMensajeCola(int32_t Id_C_Mensajes)
{
   msgrcv (Id_C_Mensajes, (struct msgbuf *)&m,
                sizeof(m.dato_numerico) + sizeof(m.mensaje), 
                m.id_mensaje, 0);
}
/**
 * @brief controla la cola de mensajes, en este caso la elimina la del id pasado
 * @param int Id_C_Mensajes
 * */
void controlCola(int32_t Id_C_Mensajes)
{
    msgctl (Id_C_Mensajes, IPC_RMID, (struct msqid_ds *)NULL);
}