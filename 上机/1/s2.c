#include "unp.h"

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE];
    socklen_t addrlen = sizeof(cliaddr);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8001);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    while(1) {
        connfd = Accept(listenfd, (SA *)&cliaddr, &addrlen);
        printf("Connection from: %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        while(1){
            int n = read(connfd, buff, sizeof(buff));
            if (n < 0) {
                err_sys("read error");
            } else if (n == 0) {
                printf("Client closed connection.\n");
                close(connfd);
                continue;
            }
            buff[n] = '\0';  // null terminate
            printf("Received message from client: %s\n", buff);
            snprintf(buff, sizeof(buff), "%s\r\n", buff);
            write(connfd, buff, strlen(buff));
        }
        // Close the connection
        close(connfd);
    }

    return 0;
}
