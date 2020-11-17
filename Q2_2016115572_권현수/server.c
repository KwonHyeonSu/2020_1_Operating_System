#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 7777
#define BUF_SIZE 1024

int i=0;
int n=0;
int ans1 = -1;
int ans2 = -1;
int c_socket1 = -1;
int c_socket2 = -1;
int again_flag = 1;
int gamestart_flag = 0;

pthread_mutex_t mutex_lock;

void* socket_thread(void* para);
void* game_thread();

int main(){
	int server_socket;
	int client_socket[2];
	int client_addr_size;
	int thr_id;
	int option;

	
	pthread_t thread1;
	pthread_t thread2;
	
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	
	pthread_mutex_init(&mutex_lock,NULL);
	pthread_mutex_lock(&mutex_lock);
	if(-1 == (server_socket = socket(PF_INET, SOCK_STREAM, 0))){
		perror("socket error ");
		exit(1);
	}
	
	option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in))){
		perror("bind error ");
		exit(1);
	}
	while(i!=2){
		printf("Listening %d client...\n",i+1);
		if(-1 == listen(server_socket, 3)){
			perror("listen error ");
			exit(1);
		}
		
		client_addr_size = sizeof(client_addr);
	
	
		client_socket[i] = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
		if(-1 == client_socket[i]){
			perror("accept error ");
			exit(1);
		}
		
		thr_id = pthread_create(&thread1, NULL, socket_thread, (void*)&client_socket[i]);
		if(thr_id<0){
			perror("thread create error ");
			exit(1);
		}
		
		printf("%d CLIENT ACCEPTED / THREAD CREATED\n", i+1);
		i++;
	}
	
	while(again_flag != 0){
		thr_id = pthread_create(&thread2, NULL, game_thread, NULL);
		if(thr_id<0){
			perror("thread create error ");
			exit(1);
		}
		pthread_join(thread2, NULL);
	}
	pthread_join(thread1, NULL);
	close(server_socket);
	printf("\nserver_socket closed\n");
	pthread_mutex_destroy(&mutex_lock);
	
	return 0;
}

void replay(){
	n = 0;
	ans1 = -1;
	ans1 = -1;
}

void* game_thread(){
	pthread_mutex_lock(&mutex_lock);
	
	if(ans1==ans2){
		printf("\nDRAW! REGAME!\n\n");
		send(c_socket1,"again",BUF_SIZE,0);
		send(c_socket2,"again",BUF_SIZE,0);
		send(c_socket1,"GAMESTART",BUF_SIZE,0);
		send(c_socket2,"GAMESTART",BUF_SIZE,0);
		replay();
		return NULL;
	}
	else if(ans1==0&&ans2==1 || ans1==1&&ans2==2|| ans1==2&&ans2==0){ //ans2 win
		printf("ID %d WIN!\n", c_socket2);
		send(c_socket2, "win", BUF_SIZE, 0);
		send(c_socket1, "lose", BUF_SIZE, 0);
		again_flag = 0;
		return NULL;
	}
	else if(ans1==1&&ans2==0 || ans1==2&&ans2==1|| ans1==0&&ans2==2){//ans1 win
		printf("ID %d WIN!\n", c_socket1);
		send(c_socket2, "lose", BUF_SIZE, 0);
		send(c_socket1, "win", BUF_SIZE, 0); 
		again_flag = 0;
		return NULL; 
	}
	printf("exit thread2\n");
	pthread_exit(NULL);
}

void* socket_thread(void* para){
	int c_socket;
	
	char buf_snd[BUF_SIZE];
	char buf_rcv[BUF_SIZE];
	
	if(c_socket1==-1 && c_socket1 == -1){
		c_socket = *((int*)para);
		c_socket1 = c_socket;
	}else{
		c_socket = *((int*)para);
		c_socket2 = c_socket;
	}
	
	sprintf(buf_snd, "\nYour ID is %d\n", c_socket);
	send(c_socket, buf_snd, BUF_SIZE, 0);
	
	while(i!=2){
	
	}
	
	strcpy(buf_snd,"GAMESTART");
	send(c_socket,buf_snd,BUF_SIZE,0);
	
	if(gamestart_flag == 0) {
		gamestart_flag = 1;
		printf("\nGame Start\n\n");
	}
	
	while(again_flag == 1){
		while(recv(c_socket, buf_rcv, BUF_SIZE, 0)){
			if(strcmp(buf_rcv, "QUIT")==0){
				break;
			}
			else{
				n++;
				if(c_socket == c_socket1) ans1 = atoi(buf_rcv);
				else if(c_socket == c_socket2) ans2 = atoi(buf_rcv);
				//if(ans1==-1 && ans2==-1) ans1 = atoi(buf_rcv);
				//else ans2 = atoi(buf_rcv);
				if(n==2) break;
			}
		
		}
		pthread_mutex_unlock(&mutex_lock);
	}
	while(recv(c_socket, buf_rcv, BUF_SIZE, 0)){
	
	}

	pthread_exit(NULL);
}





