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
	/*
    if(ret == 0){
        printf("success to connect!\n");
    }
	*/
	/////
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