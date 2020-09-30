#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h> //inet_atonを使うために必要
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
const int N = 1024;
int main(int argc, char *argv[]){
    if(argc != 3){
        printf("invalid number of words.\n");
        exit(1);
    }
    char *filename = argv[1];
    int port_number = atoi(argv[2]);
    if(port_number == 0){
        printf("incorrect port number\n");
        exit(1);
    }
    int s = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
	addr.sin_family = AF_INET; /* これはIPv4 のアドレスです */
	int aton = inet_aton(filename, &addr.sin_addr);; /* IP アドレスは...です */
	if(aton == 0){
		perror("open");
		exit(1);
	}
	addr.sin_port = htons(port_number); /* ポートは...です */
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr)); /* 遂にconnect */
    if(ret == -1){
        perror("connect");
        exit(1);
    }
    if(ret == 0){
        printf("success to connect!\n");
    }
    unsigned char data[N];
    while(1){
		int n = read(0,data,N);
        if (n == -1){perror("send");exit(1);}
		if (n>0){write(s,data,n);}

		int k = read(s,data,N);
        if (k == -1){perror("recv");exit(1);}
		if (k>0){write(0,data,n);}	

		if(n == 0) printf("END\n");
        if(n == 0) break;
    }
    shutdown(s,SHUT_WR);
	printf("END\n");
    close(s);
}
