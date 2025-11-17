#include        <sys/types.h>   /* basic system data types */
#include        <sys/socket.h>  /* basic socket definitions */
#include        <sys/time.h>    /* timeval{} for select() */
#include        <time.h>        /* timespec{} for pselect() */
#include        <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include        <arpa/inet.h>   /* inet(3) functions */
#include        <errno.h>
#include        <fcntl.h>        /* for nonblocking */
#include        <netdb.h>
#include        <signal.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <sys/stat.h>    /* for S_xxx file mode constants */
#include        <sys/uio.h>     /* for iovec{} and readv/writev */
#include        <unistd.h>
#include        <sys/wait.h> 
#include        <sys/un.h>      /* for Unix domain sockets */

#define MAXLINE		4096    /* max text line length */
#define	SERV_PORT_STR	"9877"
#define	SERV_PORT	 9877	
#define SA     struct sockaddr



#define MAXLINE         4096    /* max text line length */
#define LISTENQ         1024    /* 2nd argument to listen() */


#define 	Socket 	socket
#define 	Bind	bind
#define		Listen	listen
#define		Accept	accept
#define		Write	write
#define		Close	close
#define 	Fork	fork


void err_quit(char * s)
{
        printf("%s \n", *s);
        exit(1);
}
void err_sys(char * s)
{
        printf("%s \n", *s);
        exit(1);
}


static ssize_t
my_read(int fd, char *ptr)
{
	static int	read_cnt = 0;
	static char	*read_ptr;
	static char	read_buf[MAXLINE];

	if (read_cnt <= 0) 
	{
	     again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) 
		{
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	int	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) 
	{
		if ( (rc = my_read(fd, &c)) == 1) 
		{
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} 
		else if (rc == 0) 
		{
			if (n == 1)
				return(0);	/* EOF, no data read */
			else
				break;		/* EOF, some data was read */
		} 
		else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}


ssize_t                                         /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
        size_t          nleft;
        ssize_t         nwritten;
        const char      *ptr;

        ptr = vptr;
        nleft = n;
        while (nleft > 0) 
	{
                if ( (nwritten = write(fd, ptr, nleft)) <= 0) 
		{
                        if (errno == EINTR)
                                nwritten = 0;           /* and call write() again */
                        else
                                return(-1);             /* error */
                }

                nleft -= nwritten;
                ptr   += nwritten;
        }

        return(n);
}

