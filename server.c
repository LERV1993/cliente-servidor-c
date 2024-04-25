/*

SERVER.C
Este un programa de ejemplo de socket server
Este es el encabezado que usaremos por defecto

gcc ./server.c -o server  --  compilo el ejecutable

./server puerto -- Ejecuto el compilado @Puerto: valores mayores a 1 puede recibir varios
error en bind()  // printea un error que se explicara a futuro.

*/

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <string.h>

#include <unistd.h>

void crear_socket(char **argv)
{
    // Defino variables donde atoi es el parametro que se le pasa por linea de comandos
    int server_socket, client_socket, longitud_cliente, puerto;

    puerto = atoi(argv[1]);

    // Se necesitan dos estructuras del tipo sockaddr
    // La primera guarda info del server
    // La segunda del cliente
    struct sockaddr_in server;
    struct sockaddr_in client;

    // Configuracion del servidor
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;         // Familia TCP/IP
    server.sin_port = htons(puerto);     // Puerto
    server.sin_addr.s_addr = INADDR_ANY; // Cualquier cliente puede conectarse

    printf("Configuración finalizada.\n");

    // Defino el socket del server
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Error: Socket devolvio error.\n");
        exit(-1); // Salgo del programa
    }

    printf("Se ha creado el socket.\n");

    // Avisar al sistema que se creo un socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        printf("error en bind() \n");
        exit(-1); // Salgo del programa
    }

    printf("Bind exitoso. \n");

    // Creo la escucha y defino la cantidad de variables antes de devolver refused
    if (listen(server_socket, 10) == -1)
    {

        printf("error en listen() \n");
        exit(-1); // Salgo del programa
    }

    printf("Servidor escuchando: %d. \n", puerto);

    while (1)
    {

        longitud_cliente = sizeof(struct sockaddr_in);

        // Acepto la conexión y creo el socket del cliente (File Descriptor)
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client, &longitud_cliente)) == -1)
        {

            printf("Error en accept() \n");
            exit(-1); // Salgo del programa
        }

        // send(client_socket, "Conectado exitosamente. \n", 26, 0);
        // close(client_socket);
        // printf("Cierro la conexión del cliente. \n");

        // Creo un nuevo proceso hijo para manejar la conexión
        pid_t pid = fork();
        // Proceso hijo
        if (pid == 0)
        {

            printf("Proceso hijo creado con exito. \n");

            close(server_socket);
            printf("Cierro la conexión del servidor. \n");  // No entiendo porque
            send(client_socket, "Conectado exitosamente. \n", 26, 0);

            // Para poder revisar del lado del server que se respondió la solucitud.
            printf("Se ha respondido a un cliente. \n");
            exit(0);

        }
        // Error al crear el proceso hijo
        else if (pid < 0)
        {
            printf("Error al crear el proceso hijo. \n");
            exit(1);
        }
        else
        {
            // Proceso padre
            // El proceso padre no necesita el socket del cliente
            close(client_socket);
        }
    }

    close(server_socket);
    printf("Cierro la conexión del servidor. \n");
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        crear_socket(argv);
    }
    else
    {

        printf("Se debe ingresar el puerto como parametro. \n");
    }

    return 0;
}
