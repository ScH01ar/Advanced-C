#include	"unp.h"

int
main(int argc, char **argv)
{
    int                 listenfd, connfd, reuse;
    struct sockaddr_in	servaddr, cliaddr;
    char                buff[MAXLINE];
    socklen_t  addrlen;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(8001);

    Setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    addrlen=sizeof(cliaddr);

    for ( ; ; ) {
        connfd = Accept(listenfd, (SA *) &cliaddr, &addrlen);
        snprintf(buff, sizeof(buff), "%s %s:%d\r\n","Your ip and port:",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port );
        printf("Connection from: %s:%d\r\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        write(connfd, buff, strlen(buff));
        close(connfd);
    }
}
