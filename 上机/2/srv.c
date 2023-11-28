#include "unp.h"
#include <sys/select.h>

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE],sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t addrlen = sizeof(cliaddr);

    fd_set readfds, writefds;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8001);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    connfd = Accept(listenfd, (SA *)&cliaddr, &addrlen);
    printf("Connection from: %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    while (1) {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(connfd, &readfds);
        FD_SET(connfd, &writefds);
        FD_SET(STDIN_FILENO, &readfds); 
        int maxfd = connfd + 1;

        int ready = select(maxfd, &readfds, &writefds, NULL, NULL);

        if (ready < 0) {
            err_sys("select error");
        }

        if (FD_ISSET(connfd, &readfds)) {
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
        }


        // if (FD_ISSET(connfd, &writefds)) {
        //     printf("Enter a message to send (or -1 to exit): ");
        //     if (fgets(sendline, MAXLINE, stdin) == NULL)
        //         break;  // fgets returns NULL on error or EOF

        //     if (strcmp(sendline, "-1\n") == 0) {
        //         printf("Exiting \n");
        //         break;
        //     }

        //     write(connfd, sendline, strlen(sendline));
        // }
        // close(connfd);
        if (FD_ISSET(STDIN_FILENO, &readfds)){
            if (fgets(sendline, MAXLINE, stdin) == NULL)
                break;  
            write(connfd, sendline, strlen(sendline));
        }
    }

    return 0;
}
