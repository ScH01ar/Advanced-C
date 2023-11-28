#include "unp.h"
#include <sys/select.h>

int main(int argc, char **argv) {
    int sockfd, n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
        err_quit("usage: %s <IPaddress>", argv[0]);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8001);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");

    fd_set readfds, writefds;

    while (1) {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);

        FD_SET(sockfd, &readfds);
        FD_SET(sockfd, &writefds);
        FD_SET(STDIN_FILENO, &readfds); 

        int maxfd = sockfd + 1;

        int ready = select(maxfd, &readfds, &writefds, NULL, NULL);

        if (ready < 0) {
            err_sys("select error");
        }

        if (FD_ISSET(sockfd, &readfds)) {
            printf("read\n");
            int n = read(sockfd, recvline, sizeof(recvline));
            if (n < 0) {
                err_sys("read error");
            } else if (n == 0) {
                printf("Server closed connection.\n");
                break;
            }
            recvline[n] = '\0';  // null terminate
            printf("Received message from server: %s\n", recvline);
        }

        // if (FD_ISSET(sockfd, &writefds)) {
        //     printf("%d\n",ready);
        //     printf("Enter a message to send (or -1 to exit): ");
        //     if (fgets(sendline, MAXLINE, stdin) == NULL)
        //         break;  // fgets returns NULL on error or EOF

        //     if (strcmp(sendline, "-1\n") == 0) {
        //         printf("Exiting \n");
        //         break;
        //     }

        //     write(sockfd, sendline, strlen(sendline));
        // }
        if (FD_ISSET(STDIN_FILENO, &readfds)){
            if (fgets(sendline, MAXLINE, stdin) == NULL)
                break;  // fgets returns NULL on error or EOF
            write(sockfd, sendline, strlen(sendline));
        }
    }

    close(sockfd);
    exit(0);
}
