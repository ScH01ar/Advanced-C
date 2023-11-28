#include	"unp.h"
void
log_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen, char sendline[MAXLINE])
{
	int	n;

    char	recvline[MAXLINE + 1];

	Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
    while (1)
    {
        n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        
        recvline[n] = 0;	
        Fputs(recvline, stdout);
        printf(" log in \n");

    }
}

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
    char *              name;           


	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8888);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    name=argv[2];
    
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	log_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr),name);

	exit(0);
}
