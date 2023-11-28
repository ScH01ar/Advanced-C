#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr, cliaddr;
    int			        n;
	socklen_t	        len;
	char		        mesg[MAXLINE];
    char*               name;

    struct sockaddr_in cliaddr_array[100];
    int cliaddr_count = 0;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(8888);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    for ( ; ; ) {
		len = sizeof(cliaddr);
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, (SA *)&cliaddr, &len);
        printf("Connection from: %s:%d==>the usr is %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port),mesg);
        name=mesg;
        memcpy(&cliaddr_array[cliaddr_count], &cliaddr, sizeof(struct sockaddr_in));
        cliaddr_count++;

        for (int i = 0; i < cliaddr_count-1; i++)
        {
            Sendto(sockfd, name, n, 0, (SA *)&cliaddr_array[i], len);
        }
        
	}
}
