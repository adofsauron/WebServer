#include "Myserver.h"
#include "Base.h"


int main(int argc, char** argv)
{
	if(argc<3)
	{
		printf("error!\n");
		printf("myserver.bin  <ip>  <port>\n");
		exit(0);
	}

	char *ip = argv[1];
	int port = atoi(argv[2]);
	int logfd = getLogFd(port);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in  address;
	bzero(&address, sizeof(address));

	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip, &address.sin_addr );
	address.sin_port = htons(port);

	int flag;
	flag = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
//	if(flag<=0)
	
//		assert(FALSE);
	flag = listen(sockfd, 5);
//	if(!flag)
//		assert(FALSE);

	struct sockaddr_in new_address;
	char buf[MAXSIZE];
	char logbuf[MAXSIZE];
	char back[MAXSIZE];
	struct stat stat_buf;

	struct epoll_event events[EVENTS_NOMBER];
	int epollfd = epoll_create(5);
	
	addEpollfd(epollfd, sockfd);
	
	int n=0;
	while(1)
	{
		int ret = epoll_wait(epollfd, events, EVENTS_NOMBER, -1);
		int i;
		for(i=0; i<ret; i++)
		{
			int retfd = events[i].data.fd;
			
			if(retfd==sockfd)
			{
				struct sockaddr_in client_address;
				socklen_t client_addrlength = sizeof(client_address);
				int connfd = accept(sockfd, (struct sockaddr*)&client_address, &client_addrlength);
				addEpollfd(epollfd, connfd);
			}
			else if((events[i].events == EPOLLIN) && ((retfd != sockfd)))
			{
				int size = recv(retfd, buf, sizeof(buf), 0);
				buf[size] = '\0';
				int x = n++;
				
				sprintf(logbuf,"\n[%d]%s\n\n%s[%d]%s\n", x,START,buf,x,OVER);
				printf("%s", logbuf);
				logInfo(logfd ,logbuf);
				
				bzero(back, sizeof(back));
				char path[128];
				memset(path, 0,sizeof(path));
				getPath(buf, path);

				if(strcmp(path, "favicon.ico") == 0)
				{
						continue;
				}	
					
				char local[128];
				memset(local, 0,sizeof(local));
			#ifdef  HIDDEN		
				sprintf(local, "%s%s",VIEW, path);
			#else		
				sprintf(local, "%s", path);
			#endif		
				int fd = getFd(local);
				if(fd<0)
				{
						strcpy(back, "no this page!!!"); 
				}
				else
				{
						readIo(fd, back);
				}
				
				fstat(fd, &stat_buf);
				send(retfd, back, strlen(back), 0);
				delEpollfd(epollfd, retfd);
				close(retfd);
				
			}
			else
			{
				printf("something happen...\n");
			}
		}
	}
	
	close(sockfd);
    close(logfd);
	return 0;
}