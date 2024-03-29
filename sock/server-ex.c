#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define SOCK_PATH "testing socket"
#define MAX_SIZE 1024
typedef struct message {
	int type;
	int operation;
	int value[2];
} message;

int res;

void *th_func(void *arg){
	char buf[MAX_SIZE];
	strncpy(buf, (char *)arg, MAX_SIZE);
	int data[2];
	char oper;

	data[0] = atoi(&buf[0]);
	oper = buf[1];
	data[1] = atoi(&buf[1]);

	if(oper == '+')
		res = data[0] + data[1];
	else if (oper == '-')
		res = data[0] - data[1];
	else if(oper == '*')
		res = data[0] * data[1];
	else if(oper = '/')
		res = data[0] / data[1];
	return res;
}


int main(void){
	int s, s2, len;
	 unsigned int t;
  struct sockaddr_un local, remote;
		         char str[100];
			      double res;
			       pthread_t p_thread;
			   
				         int thr_id;
			         int status;
				 
					       if((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
						            perror("socket");
						             exit(1);
							      }
				  
					        local.sun_family = AF_UNIX;
				        strcpy(local.sun_path, SOCK_PATH);
				         // 소켓이 이미 있을 경우 bind가 되지 않으므로 삭제 시켜준다
						     unlink(local.sun_path);
					      len = strlen(local.sun_path) + sizeof(local.sun_family);
					  
						       // 서버프로세스와 소켓을 연결
						        if(bind(s, (struct sockaddr *)&local, len) == -1){
								          perror("bind");



	
int main(void) {
	int s, s2, len;
	message m;
	unsigned int t;
	struct sockaddr_un local, remote;
	char str[100];

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

	for(;;) {
		int done, n;
		printf("Waiting for a connection...\n");
		t = sizeof(remote);

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
				}
			}
		} while(!done);

		close(s2);
	}

	return 0;
}
