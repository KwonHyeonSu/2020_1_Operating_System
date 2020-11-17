#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 7777
#define BUF_SIZE 1024

int main(){
	int client_socket;
	int input_int;
	
	char input[BUF_SIZE];
	char buf_snd[BUF_SIZE];
	char buf_rcv[BUF_SIZE];
	
	struct sockaddr_in server_addr;
	
	if(-1 == (client_socket = socket(PF_INET, SOCK_STREAM, 0))){
		perror("Socket error");
		exit(1);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(-1 == connect(client_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in))){
		perror("connect error ");
		exit(1);
	}
	
	printf("CONNECTED!\n");
	recv(client_socket, buf_rcv, BUF_SIZE, 0);
	printf("%s",buf_rcv);
	while(1){
		memset(buf_rcv, 0, BUF_SIZE);
		memset(buf_snd, 0, BUF_SIZE);
		
		recv(client_socket, buf_rcv, BUF_SIZE, 0);
		if(strcmp(buf_rcv, "GAMESTART") == 0){
			while(1){
				printf("\n-----GAME START-----");
				printf("\n(S, R, P)\n");
				printf("\nInput : ");
				scanf("%s",input);
				if(strcmp(input,"S")==0 || strcmp(input,"R")==0 || strcmp(input,"P")==0) break;
				printf("\n\x1b[31mWrong answer. Please retype.\x1b[0m\n\n");
			}
			if(strcmp(input,"S")==0) input_int = 0;
			else if(strcmp(input,"R")==0) input_int = 1;
			else if(strcmp(input,"P")==0) input_int = 2;
			 
			sprintf(buf_snd, "%d", input_int);
			send(client_socket, buf_snd, BUF_SIZE, 0);
		}
		else if(strcmp(buf_rcv, "win")==0)
		{
			strcpy(buf_snd,"QUIT");
			send(client_socket,buf_snd,BUF_SIZE,0);
			printf("\n\x1b[34mYOU WIN!\x1b[0m\n");
			break;
		}
		else if(strcmp(buf_rcv, "lose")==0)
		{
			strcpy(buf_snd,"QUIT");
			send(client_socket,buf_snd,BUF_SIZE,0);
			printf("\n\x1b[31mYOU LOSE!\x1b[0m\n");
			break;
		}
		else if(strcmp(buf_rcv, "again")==0)
		{
			printf("\nDRAW! RETRY!\n");
		}
	}
	//strcpy(buf_snd, "HELLO");
	//send(client_socket, buf_snd, BUF_SIZE, 0);

	close(client_socket);
	printf("\nclient_Socket closed\n");
	return 0;
}
