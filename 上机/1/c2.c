#include "unp.h"

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

    while (1) {
        printf("Enter a message to send (or -1 to exit): ");
        if (fgets(sendline, MAXLINE, stdin) == NULL)
            break;  // fgets returns NULL on error or EOF

        if (strcmp(sendline, "-1\n") == 0) {
            printf("Exiting \n");
            break;
        }

        write(sockfd, sendline, strlen(sendline));
        if ((n = read(sockfd, recvline, MAXLINE)) > 0) {
            recvline[n] = '\0';  // null terminate
            
            if (fputs(recvline, stdout) == EOF)
			    err_sys("fputs error");
        }
    }

    close(sockfd);
    exit(0);
}
