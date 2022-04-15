#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main(){
	int fd1,ret;
	char bur1[20] = "Go to myfile";
	char buf2[20] = "Go to stdout\n";

	fd1 = open("myfile", O_CREAT|O_TRUNC| O_RDWR,S_IRUSR| S_IWUSR);
	ret= dup(1);

	close(1);
	dup(fd1);
	close(fd1);

	write(1,bur1,20);
	close(ret);
	dup(4);
	close(4);
	write(1,buf2,20);
	return 1;
}