#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	int A = atoi(argv[1]);
	short buf;
	int count=0;
	while (1) {
		int n = read(0, &buf,sizeof(short));
		if (n == -1) { perror ("read"); exit(1);}
		if (n == 0) break;

		if (count%A==0) {
			int k  = write(1,&buf,sizeof(short));
			if (k == -1) { perror ("write"); exit(1);}
		}
		count++;
	}

	return 0;
}