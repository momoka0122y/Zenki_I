#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv){

	int A = atoi(argv[1]);
	int f = atoi(argv[2]);
	int n = atoi(argv[3]);

	for(int t=0; t<n;t++) {
		short x = A*sin(2*M_PI*f*t/44100);

		int k  = write(1,&x,2);
		if (k == -1) { perror ("write"); exit(1);}
	}
	return 0;
}