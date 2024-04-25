/*

Uso: ./client ip puerto

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{

    if (argc > 2)
    {   
        // Se definen variables
        int file_descriptor, puerto, tam;
        char buffer[50];
        char *ip;

        // ARGV: Es un vector que contiene los valores de puerto e ip. Son los argumentos ingresados al momento de correr el programa.
        puerto = atoi(argv[2]);   // ATOI: Convierte string a int
        ip = argv[1];
        int cant;  // validar recepción del server

        struct sockaddr_in server;
        struct hostent *host;

        if ((host = gethostbyname(ip)) == NULL)
        {

            printf("Error en gethostbyname()");
            exit(-1);

        }

        if ((file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {

            printf("Error: Socket devolvio error.\n");
            exit(-1); // Salgo del programa

        }

        server.sin_family = AF_INET;                                        // Familia TCP/IP
        server.sin_port = htons(puerto);                                    // Puerto
        server.sin_addr = *((struct in_addr *)host->h_addr);                // La direción a la que me quiero conectar
        bzero(&(server.sin_zero), 8); 

        if(connect(file_descriptor,(struct sockaddr *)&server,sizeof(struct sockaddr)) == -1){

            printf("Error: Connect devolvio error.\n");
            exit(-1); // Salgo del programa

        }

        if((cant = recv(file_descriptor,buffer,50,0)) == -1 ){

            printf("Error: El server no devolvió nada.\n");
            exit(-1); // Salgo del programa

        }

        buffer[cant] = '\0'; // El \* indica el final de una cadena || Dato aún sin él funciona igual

        printf("Respuesta del server: %s ", buffer); // Es como el print format de python "s" representa el tipo de dato string

        close(file_descriptor);
    }


    else {

        printf("Se debe ingresar el puerto como parametro");

    }
}