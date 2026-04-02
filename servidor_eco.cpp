#include <iostream>     // Para imprimir en terminal
#include <sys/socket.h> // Funciones principales de sockets
#include <netinet/in.h> // Estructuras de direcciones de red (sockaddr_in)
#include <unistd.h>     // Para usar la función close()
#include <cstring>      // Para manejar el tamaño de los textos (strlen)

int main() {
    // 1. CREACIÓN DEL SOCKET (El "teléfono")
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error: No se pudo crear el socket principal." << std::endl;
        return 1;
    }

    // 2. CONFIGURACIÓN DE LA DIRECCIÓN Y PUERTO
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;           // IPv4
    direccion.sin_addr.s_addr = INADDR_ANY;   // Escuchar en todas las interfaces de red
    direccion.sin_port = htons(8080);         // Puerto 8080

    // 3. AMARRAR EL SOCKET AL PUERTO (Bind)
    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        std::cerr << "Error: Fallo al hacer bind en el puerto 8080." << std::endl;
        return 1;
    }

    // 4. PONER EN MODO ESCUCHA (Listen)
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error: Fallo al iniciar la escucha (listen)." << std::endl;
        return 1;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "  Servidor TCP Iniciado en Termux       " << std::endl;
    std::cout << "  Escuchando en el puerto 8080...       " << std::endl;
    std::cout << "========================================" << std::endl;

    int addrlen = sizeof(direccion);

    // 5. BUCLE PRINCIPAL (El servidor se queda abierto)
    while (true) {
        // Esperamos a que alguien se conecte
        int new_socket = accept(server_fd, (struct sockaddr *)&direccion, (socklen_t*)&addrlen);

        if (new_socket < 0) {
            std::cerr << "Error temporal al aceptar un cliente. Reintentando..." << std::endl;
            continue; // Usamos 'continue' en lugar de 'return 1' para que el servidor no se apague
        } 

        std::cout << "\n[+] ¡Un nuevo cliente se ha conectado!" << std::endl;

        // A. Enviar el mensaje de bienvenida y pedir input
        const char *mensaje = "\n¡Bienvenido al servidor de Samuel!\nPor favor, escribe un mensaje y presiona Enter: ";
        send(new_socket, mensaje, strlen(mensaje), 0);

        // B. Crear un espacio (buffer) para recibir la respuesta
        char buffer[1024] = {0}; // Un tren vacío de 1024 vagones
        
        // C. Esperar y leer lo que el cliente escriba
        int bytes_leidos = recv(new_socket, buffer, 1024, 0);

        if (bytes_leidos > 0) {
            // Mostrar en nuestra terminal lo que nos mandaron
            std::cout << "[>] El cliente escribio: " << buffer;
            
            // D. Devolverle el mensaje al cliente (Efecto Eco)
            const char *respuesta_prefijo = "\nServidor recibio tu mensaje: ";
            send(new_socket, respuesta_prefijo, strlen(respuesta_prefijo), 0);
            send(new_socket, buffer, strlen(buffer), 0); // Reenviamos el mismo texto
        }

        // E. Despedir a este cliente y cerrar su conexión particular
        const char *despedida = "\nCerrando conexion. ¡Hasta luego!\n";
        send(new_socket, despedida, strlen(despedida), 0);
        close(new_socket);
        
        std::cout << "[-] Conexion cerrada con el cliente. Esperando al siguiente..." << std::endl;
    }

    // Esto nunca se ejecutará a menos que forcemos el cierre del programa,
    // pero es buena práctica ponerlo.
    close(server_fd);
    return 0;
}

