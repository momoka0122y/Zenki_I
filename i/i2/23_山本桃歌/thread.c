#include<stdio.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define QUEUELIMIT 10 //一度につなげられるクライアントの数
#define THREAD 10　//作成するスレッドの数。この数だけ一度に対処できる

void *server(void *arg){
    int clients;
    int s = *(int *)arg;
    struct sockaddr_in claddr;
    socklen_t claddr_size = sizeof claddr;
    char buf[2048];
    int r;
    int sen;
    if((clients = accept(s,(struct sockaddr *)&claddr,&claddr_size))<0){perror("accept error");exit(1);}
    while(1){
        memset(buf,0,sizeof(buf));
        if((r=recv(clients, buf, sizeof(buf), 0))<0){perror("recv error");exit(1);}
		write(0,buf,r);
        if((sen=send(clients,buf,r,0))<0){perror("send error");exit(1);}
    }
    close(clients);
}


int main(int argc, char* argv[]){
    int s;
    if((s=socket(AF_INET,SOCK_STREAM,0))<0){perror("socket error");exit(1);}

    int b;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    int port = atoi(argv[1]);
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ((b = bind(s,(struct sockaddr *)&addr,sizeof(addr)))<0){perror("bind error");exit(1);}

    int l;
    if((l = listen(s,QUEUELIMIT))<0){perror("listen error");exit(1);}
    pthread_t pth[THREAD];
    for(int i=0;i<THREAD;i++){
        if(pthread_create(&pth[i],NULL,server,&s)<0){perror("pthread_create error");exit(1);}
    }
    for(int i=0;i<THREAD;i++){
        if(pthread_join(pth[i],NULL)<0){perror("pthread_join error");exit(1);}
    }

    
    return 0;
}