#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){

	int fd = open(argv[1],O_RDONLY);
	if (fd == -1) { perror ("open"); exit(1);}
	short buf;
	int count=0;
	while (1) {
		int n = read(fd, &buf,sizeof(short));
		if (n == 0) break;
		printf("%d %d\n",count,buf);
		count++;
	}
	close (fd);

	return 0;
}