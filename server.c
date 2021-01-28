/*
 * UDP chat-server
 * Компиляция:
 *  cc -Wall -O2 -o server server.c
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

/*
 * Получить данные из сокета 
 */
ssize_t recvfrom(int sockfd, void *buff, size_t nbytes, int flags,
        struct sockaddr *from, socklen_t *addrlen);

/*
 * Отправить данные через сокет
 */
ssize_t sendto(int sockfd, const void *buff, size_t nbytes, int flags,
        const struct sockaddr *to, socklen_t addrlen);

int main() {
    int serverSocket;
    int nBytes;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    char buffer[1024];

    /* Создать сокет. */
    serverSocket = Socket(PF_INET, SOCK_DGRAM, 0);

    /* Инициализировать структуру адреса сокета сервера. */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&serverAddr, 0, sizeof(serverAddr));

    /* Связать сокет с локальным адресом протокола. */
    Bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    addr_size = sizeof serverStorage;

    while (1) {
        nBytes = recvfrom(serverSocket, buffer, 1024, 0, (struct sockaddr *)&serverStorage, &addr_size);

        printf("client: %s\n", buffer);

        printf("Type: ");
        fgets(buffer, 1024, stdin);

        nBytes = strlen(buffer) + 1;

        sendto(serverSocket, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size);


    }

    return 0;
}

/*
fgets()
считывает строку текста

recvfrom() / sendto()
Обе возвращают количество записанных или прочитанных байтов

Аргумент to для функции sendto — это структура адреса сокета, содержащая адрес протокола 
sendto() для несоединенных UDP-сокетов SOCK_DGRAM. 
нужно указывать адрес назначения каждый раз 

*/