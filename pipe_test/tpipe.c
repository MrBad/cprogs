#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// testing pipe, intended for cOSiris

int main()
{
	pid_t pid;
	int fd[2], n;
	char buf1[512], buf2[512];

	pipe(fd);
	pid = fork();
	if(pid < 0) {
		return 1;
	} else if(pid == 0) {
		close(fd[1]);
		n = read(fd[0], buf2, sizeof(buf2));
		buf2[n] = 0;
		printf("read %d bytes from pipe:\n\t%s", n, buf2);
		close(fd[0]);
	} 
	else {
		close(fd[0]);
		strcpy(buf1, "This is a message from parent to child\n");
		if(write(fd[1], buf1, strlen(buf1)) != strlen(buf1)) {
			printf("Error writing to pipe\n");
			return 1;
		}
		close(fd[1]);
		wait(NULL);
	}
}
