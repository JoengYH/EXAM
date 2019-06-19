// client.c //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCK_PATH "testing socket"
typedef struct meesage { 
	int type;
	char operation;
	int value[2];
} message;

int main(void) {
	int s, t, len;
	struct sockaddr_un  remote;
	char str[100];
	message m;

	if( (s = socket(AF_UNIX, SOCK_STREAM, 0) ) == -1 ) {
		printf("socket");
		exit(1);
	}
	
	printf("Trying to connect...\n");

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	
	if( connect(s, (struct sockaddr *)&remote, len) == -1 ) {
		printf("connect");
		exit(1);
	}

	printf("Connected.\n");

	while(printf("> "), fgets(str, 100, stdin), !feof(stdin)) {
			m.type = 1;
			m.value[0] = atoi(&str[0]);
			m.operation = str[1];
			m.value[1] = atoi(&str[2]);
			if( send(s, &m, sizeof(m), 0) == -1) {
				printf("send");
				exit(1);
			}

			if( (t=recv(s,&m, 100, 0)) > 0 ) {
				printf("echo> %d\n", m.value[0]);
			}
			else {
				if (t < 0) printf("recv");
				else printf("server closed connection\n");
				exit(1);
			}
	}

	close(s);

	return 0;
}

