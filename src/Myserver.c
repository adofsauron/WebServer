#include "Myserver.h"
#include "Base.h"

int setNoneBlocking(int fd)
{
	int old = fcntl(fd, F_GETFL);
	int new = old|O_NONBLOCK;
	fcntl(fd, F_SETFL, new);
	return old;
}

void addEpollfd(int epollfd, int fd)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setNoneBlocking(fd);
}

void delEpollfd(int epollfd, int fd)
{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}



int getFd(char *name)
{
	int fd;	
	if(strcmp(name, "view/")==0)
	{
		return -1;
	}
	fd = open(name, O_RDONLY);
	return fd;
}

int readIo(int fd, char* back)
{
	char buff[MAXSIZE];
	memset(buff, 0x00,sizeof(buff));
	int n = read(fd, buff, sizeof(buff));
	memcpy(back, buff, n);
	close(fd);
	
	return n;		
}

char *getPath(char* src, char* path)
{
	char* p = strchr(src, '/');
	p++;
	char* end = strchr(p, ' ');
	int n = end-p;		
	memcpy(path, p, n);
	
	return path;		
}

int getLogFd(int port)
{
	char logpath[128];
	memset(logpath, 0, sizeof(logpath));
	sprintf(logpath, "%s%d.txt", LOGDIR, port);
	int fd;
	fd = open(logpath, O_RDWR|O_CREAT|O_APPEND);
	chmod(logpath, S_IROTH); //read by others, man chmod(2)
	if(fd<0)
		return -1;
	return fd;	
}

int logInfo(int fd, char* log)
{		
	int n = write(fd, log, strlen(log));			
	return n;
}





