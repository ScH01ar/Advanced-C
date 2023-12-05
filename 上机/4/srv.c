#include	"unp.h"


void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE]={0};

	for ( ; ; ) {
		len = clilen;
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        printf("len:%d\n",n);
        // mesg[n] = '\0'; 
        if (strcmp(mesg, "hello\n") == 0) {
            printf("666\n");
            close(sockfd);
            break; 
        }
		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}



int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr, cliaddr;
    int			        n;
	socklen_t	        len;
	char		        mesg[MAXLINE];
    char                usrname[10][MAXLINE]={0};
    int                 sourceIndex = -1;

    struct sockaddr_in cliaddr_array[100];
    int cliaddr_count = 0;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(8888);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
    for ( ; ; ) {
        char name[MAXLINE]={0};
		len = sizeof(cliaddr);
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, (SA *)&cliaddr, &len);
        char me[MAXLINE]={0};
        strcpy(name, mesg);
        name[n]='\0';
        // 登陆消息
        if (strncmp(name + strlen(name) - 3, "log", 3) == 0){
            name[strlen(name)-3]='\0';
            printf("Connection from: %s:%d==>the usr is %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port),name);
            memcpy(&cliaddr_array[cliaddr_count], &cliaddr, sizeof(struct sockaddr_in));
            // strcpy(usrname[cliaddr_count], name);
            strcpy(usrname[cliaddr_count], name);
            
            cliaddr_count++;
            puts(name);
            strcat(name," logs in\n");
            for (int i = 0; i < cliaddr_count-1; i++)
            {
                Sendto(sockfd, name, sizeof(name), 0, (SA *)&cliaddr_array[i], len);
            }
        }else if(strncmp(name, "quit", 4) == 0){
            for (int i = 0; i < cliaddr_count; i++) {
                if (memcmp(&cliaddr, &cliaddr_array[i], len) == 0) {
                    sourceIndex = i;
                    strcat(me,usrname[sourceIndex]);
                    
                    strcat(me," leave\n");
                    puts(me);
                    break;
                }
            }
            for (int i = 0; i < cliaddr_count; i++)
            {
                if (memcmp(&cliaddr, &cliaddr_array[i], len) == 0) {
                    continue;
                }
                Sendto(sockfd, me, sizeof(me), 0, (SA *)&cliaddr_array[i], len);
            }
        }
        else{
            for (int i = 0; i < cliaddr_count; i++) {
                if (memcmp(&cliaddr, &cliaddr_array[i], len) == 0) {
                    sourceIndex = i;
                    strcat(me,usrname[sourceIndex]);
                    
                    strcat(me," says: ");
                    strcat(me,name);
                    puts(me);
                    break;
                }
            }
            for (int i = 0; i < cliaddr_count; i++)
            {
                if(i==sourceIndex){
                    
                    continue;
                }
                Sendto(sockfd, me, sizeof(me), 0, (SA *)&cliaddr_array[i], len);
            }
        }
        
        
	}
}
