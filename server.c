/***********************************************************************
server.c:

 1/ Receives a lists of users from a client
 2/ Generate a goal for each user, and append goal to file init_kuang
 3/ Runs kuang
 4/ Reads result from file Success and sends back to client

***********************************************************************/

#include	"header.h"

void run_kuang(int sockfd);

int main(int argc, char **argv)
{
	int			listenfd, connfd;
	pid_t			childpid;
	socklen_t		clilen;
	struct sockaddr_in	cliaddr, servaddr;

	signal (SIGCHLD, SIG_IGN);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) 
	{
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

                if( (childpid = fork()) < 0)
                {
                        perror("fork");
                        exit(1);
                }
        
                if ( childpid  == 0)
                {
                        Close(listenfd);        /* child closes listening socke$
			run_kuang(connfd);      /* process the request */
                        Close(connfd);          /* done with this client */
                        exit(0);
                }  

		Close(connfd);		/* parent closes connected socket */
	}
}

void run_kuang(int sockfd)
{
        ssize_t         n;
        char            line[MAXLINE];
	char		end_signal[4];
	char            pretext[25];
	char		endtext[20];
	char		goal[80];
	int k;

	FILE * fp   = fopen("init_kuang","a");	
	FILE * outf;

	strcpy(pretext,"./addto uids ");
	strcpy(endtext," DO ANYTHING \n");

	if (fp == NULL)
	{
		printf("error in opening init_kuang at server site\n");
		exit(1);
	}
	
	strcpy(end_signal,"EOF");
	end_signal[3] = '\n' ;
        while(1)
	{
		n = readline(sockfd, line, MAXLINE);
		
		if (strncmp(line,end_signal,3) != 0)
		{
			strcpy(goal,pretext);	
			strncat(goal,line,(strlen(line)-1));
			strncat(goal,endtext,strlen(endtext));
			fprintf(fp,goal);
			printf("%s", goal);
		}
		else
			break;
        }
	
	fclose(fp);

	k = system("./kuang");

	outf = fopen("Success", "r");

	if (outf == NULL)
	{
		printf("error in open Success at server\n");
		exit(1);
	}
	while (1)
	{	
		if (fgets(line, MAXLINE, outf) == NULL)
			break;

		writen(sockfd, line, strlen(line));
	}
	fclose(outf);

}

