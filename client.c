/*
 * UDP chat-server
 * Компиляция:
 *  cc -Wall -O2 -o client client.c
 *
 */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>



/*
 * Конфигурация сервера.
 */
#define PORT 1027
#define BACKLOG 5
#define MAXLINE 256

#define SA struct sockaddr


// Обработчик фатальных ошибок.
void error(const char *s) {
    perror(s);
    exit(-1);
}

/*
 * Функции-обертки.
 */
int Socket(int domain, int type, int protocol) {
    int rc;

    rc = socket(domain, type, protocol);
    if(rc == -1) error("socket()");

    return rc;
}

int Bind(int socket, struct sockaddr *addr, socklen_t addrlen) {
    int rc;

    rc = bind(socket, addr, addrlen);
    if(rc == -1) error("bind()");

    return rc;
}

int main() {
    int clientSocket;
    int nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = Socket(PF_INET, SOCK_DGRAM, 0);

    /* Инициализировать структуру адреса сокета. */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&serverAddr, 0, sizeof(serverAddr));

    addr_size = sizeof serverAddr;

    while (1) {
        printf("Type: ");
        fgets(buffer, 1024, stdin);

        nBytes = strlen(buffer) + 1;

        sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);

        nBytes = recvfrom(clientSocket, buffer, 1024, 0, NULL, NULL);

        printf("server: %s\n", buffer);
    }

    return 0;
}