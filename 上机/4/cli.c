#include "unp.h"
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
void chat_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen, char *name) {
    int n, maxfdp1;
    fd_set readfds;
    char sendline[MAXLINE] = {0}, recvline[MAXLINE + 1];

    strcat(sendline, name);
    strcat(sendline, "log");


    Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);



    while (1) {
        FD_ZERO(&readfds);

        maxfdp1 = sockfd + 1; 
        FD_SET(sockfd, &readfds);     
        FD_SET(STDIN_FILENO, &readfds); 
        int ready = select(maxfdp1, &readfds, NULL, NULL, NULL);
        if (ready < 0) {
            err_sys("select error");
        }
        if (FD_ISSET(sockfd, &readfds)) {

            n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            if (n == 0) {
                fprintf(stderr, "Server terminated prematurely\n");
                return;
            }

            recvline[n] = '\0'; 
            fputs(recvline, stdout);
        }
        if (FD_ISSET(STDIN_FILENO, &readfds)) {

            if (Fgets(sendline, MAXLINE, fp) == NULL) {
                return; 
            }
            if (strcmp(sendline, "quit\n") == 0) {
                Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
                
                close(sockfd);
                break; 
            }
            Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
    }

        
    }
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IPaddress> <name>\n", argv[0]);
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    chat_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr), argv[2]);

    exit(0);
}
