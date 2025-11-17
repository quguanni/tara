/***********************************************************************
client.c:

 1/ Sends a lists of users to server(s)
 2/ Receives result from server(s) and appends to file 
	Success_from_(server_name)

***********************************************************************/


#include	"header.h"

  
void read_kuang(FILE *user_fp, int sockfd, FILE *success_fp);

int main(int argc, char * argv[3])
{
	int			sockfd;
	struct sockaddr_in	servaddr;
	FILE 			* user_fp;
	FILE 			* success_fp;
	char 			tmp_str[50];

	user_fp = fopen(argv[2], "r");	/* this file contents a list of 
					remote users' names appear in .rhosts */ 

	strcpy(tmp_str, "Success_from_");
	strncat( tmp_str, argv[1], (strlen(argv[1])) );

	success_fp = fopen(tmp_str, "w");	/* this is file Success_from_(server_name)*/ 
          

	if (user_fp == NULL)
	{
		printf("error in open file %s\n", argv[1]);
		exit(1);
	}

	if (success_fp == NULL)
	{
		printf("error in open file %s\n", tmp_str);
		exit(1);
	}
	
	if (argc != 3)
	{
		err_quit("usage: ./k_client <remote server> <user file> \n");
		exit(1);
	}

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	read_kuang(user_fp , sockfd, success_fp);
	fclose(user_fp);
	fclose(success_fp);

	exit(0);
}

/*****************************************************************
Function read_kuang():
   1/ write the content of <user file> to socket
   2/ append the receiving result from server to file Success_server

******************************************************************/
 
void read_kuang(FILE *user_fp, int sockfd, FILE *success_fp)
{
        char    sendline[MAXLINE], recvline[MAXLINE];
	char	end_line[4];


	strcpy(end_line,"EOF");	     /* flag to let server know 
					that it's end of input file */

	end_line[3] = '\n';

        while (1)
	{
		if (fgets(sendline, MAXLINE, user_fp) == NULL)
		{
			writen(sockfd, end_line,strlen(end_line));
			break;
		} 
            	writen(sockfd, sendline, strlen(sendline));
	}


	/* read result from socket and append to a file */
	while ( readline( sockfd, recvline, MAXLINE ) != 0 )
	{
		fputs(recvline, success_fp);
	}


}

