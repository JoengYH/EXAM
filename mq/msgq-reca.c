#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFF_SIZE 1024

typedef struct{
	long data_type;
	int data_num;
	char data_buff[BUFF_SIZE];
}msg_t;

int main(int argc, char *argv[]){
	int msqidA;
	msg_t data, dataA;

	if(argc < 2){
		perror("parameter for the msg type is required");
		exit(1);
	}
	
	if(-1 == (msqidA = msgget((key_t)159631, IPC_CREAT | 0666))){
		perror("msgget() error");
		exit(1);
	}
	
	while(1){
		if(-1 == msgrcv(msqidA, &data, sizeof(msg_t) - sizeof(long), atoi(argv[1]), 0)){
			perror("msgrcv() error");
			exit(1);
		}
		printf("%d - %s\n", data.data_num, data.data_buff);
	}
	return 0;
}
