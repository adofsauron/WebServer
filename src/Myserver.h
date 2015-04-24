#ifndef _MYSERVER_H_
#define _MYSERVER_H_
	
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <assert.h>

	 
int	 	getFd(char* name); 
int	 	readIo(int fd, char* back);
char*	getPath(char* src, char* path);
int 	getLogFd(int port);
int 	logInfo(int fd, char* log);
int 	setNoneBlocking(int fd);
void 	addEpollfd(int epollfd, int fd);
void 	delEpollfd(int epollfd, int fd);


#endif //_MYSERVER_H_
