#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	int n=256;
	char a[n];

	for(int i = 0; i <n; i++ ) {
		a[i] = i;
	}

	int fd = open(argv[1], O_WRONLY| O_CREAT | O_TRUNC,0644);
	if (fd == -1) { perror ("open"); exit(1);}
	int k  = write(fd,a,n);
	if (k == -1) { perror ("write"); exit(1);}
	close(fd);
	return 0;
}