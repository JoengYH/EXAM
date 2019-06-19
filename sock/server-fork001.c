#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "testing socket"

typedef struct message {
	int type;
	int operation;
	int value[2];
} message;

int main(void) {
	int s, s2, len;
	message m;
	unsigned int t;
	struct sockaddr_un local, remote;
	char str[100];
	pid_t pid;
	int sum = 0;

	if( (s = socket(AF_UNIX, SOCK_STREAM, 0) ) == -1 ) {
		printf("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	
	if( bind(s, (struct sockaddr *)&local, len) == -1) {
		printf("bind");
		exit(1);
	}

	if( listen(s, 5) == -1) {
		printf("listen");
		exit(1);
	}

	int num = getppid();
	
	for(;;) {
		int done, n;
		t = sizeof(remote);
		printf("Waiting for a connection...\n");
		if( (s2 = accept(s, (struct sockaddr *)&remote, &t) ) == -1 ) {
			printf("accept");
			exit(1);
		}

		printf("connected.\n");

		done = 0;
		do {
			n = recv(s2, &m, sizeof(m), 0);
			if (n <= 0) {
				if (n < 0) printf("recv");
				done = 1;
			}
			if (!done) {
				m.type = 2;
				if((pid = fork()) < 0) {
					perror("fork");
				}
				else if(pid > 0){
					if(send(s2, &m, sizeof(m), 0) < 0) {
						printf("send");
						done = 1;
					}
					if(m.operation == '+'){
						sum  = m.value[0] + m.value[1];
						printf("child : %d + %d = ?\n", m.value[0], m.value[1]);
					}
					else if(m.operation == '-'){
						sum = m.value[0] - m.value[1];
						printf("child : %d - %d = ?\n", m.value[0], m.value[1]);
					}
					else if(m.operation == '*'){
						sum = m.value[0] * m.value[1];
						printf("child : %d * %d = ?\n", m.value[0], m.value[1]);
					}
					else{
						sum = m.value[0] / m.value[1];
						printf("child : %d / %d = ?\n", m.value[0], m.value[1]);
					}

				}
				else{ 
					printf("parent : Result is %d!!\n", sum);
				}
			}
		}while(!done);

		close(s2);
	}

	return 0;
}
