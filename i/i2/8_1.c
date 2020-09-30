#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h> //inet_atonを使うために必要
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
const int N = 30000;
int main(int argc, char *argv[]){
	printf("waiting for connect...");
	int port_number = 50000;
    if(argc > 1){
		port_number = atoi(argv[1]);
		if(port_number == 0){
			printf("incorrect port number\n");
			exit(1);
		}
    }
    
	int ss = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr; /* 最終的にbind に渡すアドレス情報 */
	addr.sin_family = AF_INET; /* このアドレスはIPv4 アドレスです */
	addr.sin_port = htons(port_number); /* ポート...で待ち受けしたいです */
	addr.sin_addr.s_addr = INADDR_ANY; /* どのIP アドレスでも待ち受けしたいです */
	bind(ss, (struct sockaddr *)&addr, sizeof(addr));
 
	listen(ss, 10); /* 接続可能宣言 */

	struct sockaddr_in client_addr;
	socklen_t len = sizeof(struct sockaddr_in);
	printf("waiting for connect...");
	int s = accept(ss, (struct sockaddr *)&client_addr, &len); /* 接続が来るまで待機 */
	/*
	char Hello[32]= "You are connected\n";
	write(s,Hello,32);
	*/
	FILE  *fp_in;
	char  *cmd_rec = "rec -t raw -b 16 -c 1 -e s -r 44100 - 2>/dev/null";
	if ( (fp_in=popen(cmd_rec,"r")) ==NULL) {
		perror ("can not rec");
		exit(EXIT_FAILURE);
	}
	FILE  *fp_out;
	char  *cmd_play = "play -t raw -b 16 -c 1 -e s -r 44100 -";
	if ( (fp_out=popen(cmd_play,"w")) ==NULL) {
		perror ("can not play");
		exit(EXIT_FAILURE);
	}
	

	unsigned char data_rec[N];
    unsigned char data_play[N];
    while(1){
		int k = fread(data_rec, 1, N, fp_in);
		if (k == 0) break;
		write(s,data_rec,N);

		int n = read(s,data_play,N);
        if (n == -1){perror("read");exit(1);}
		if (n>0){
			fwrite(data_play, 1, N, fp_out);
		}
		if(n == 0) printf("END\n");
        if(n == 0) break;

    }
	shutdown(s,SHUT_WR);
	printf("END\n");
    close(s);
}