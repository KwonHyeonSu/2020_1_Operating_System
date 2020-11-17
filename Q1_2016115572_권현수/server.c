#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //소켓 헤더
#include <netinet/in.h> //소켓 헤더
#include <pthread.h> //쓰레드 헤더
#include <unistd.h>

#define PORT 7777 //포트번호 7777
#define BUF_SIZE 1024 //BUF_SIZE = 1024

void* socket_thread(void* para);
int main(int argc, char** argv){ //argv[1]로 포트번호 정할 수 있음.
	
	int server_socket; //서버 소켓 디스크립터 변수
	int client_socket; //클라이언트 소켓 디스크립터 변수
	int client_addr_size; //client_addr의 크기 변수
	int n=0; //접속한 클라이언트 갯수
	int option; //bind에러 해결하기 위한 변수
	pthread_t thr_id; //스레드 변수
	
	struct sockaddr_in server_addr; //sockaddr_in 서버 구조체
	struct sockaddr_in client_addr; //sockaddr_in 클라이언트 구조체

	//PF_INET : TCP/IP사용 , SOCK_STREAM : TCP방식, 0 : 타입에서 미리 정해졌을 때.
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	option = 1; //SO_REUSEADDR 의 옵션값을 TRUE로 바꿔줌
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	
	if(server_socket == -1){
		perror("Socket Create Error : ");
		exit(1);
	}
	printf("Socket Create Complete...\n");

	//server_addr 구조체 정의
	server_addr.sin_family = AF_INET; //AF_INET : IP 주소 체계 지칭
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY : IP 주소값 자동으로 할당
	
	if(argc == 1) server_addr.sin_port = htons(PORT);//htons : PORT(=5555)를 HOST(리틀엔디안)형식에서 NETWORK(빅엔디안)형식으로 변환
	else server_addr.sin_port = htons((atoi)(argv[1]));

	if(-1==bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
		perror("Bind error : ");
		exit(1);
	}
	printf("Bind Complete...\n");
	
	//클라이언트를 계속 받을 수 있게 무한루프를 돌림
	while(1){

	 	//client의 요청을 기다림.
		if(-1==listen(server_socket, 3)){
			perror("Listen error : ");
			exit(1);
		}
		printf("Listening...\n");
		
		client_addr_size = sizeof(client_addr);
		//요청을 수락함.
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
		if(client_socket == -1){
			perror("Accept error : ");
			exit(1);
		}
		printf("Accept Complete...%d\n", ++n);
		
		//스레드를 생성하여 send, recv함수 수행.
		pthread_create(&thr_id, NULL, socket_thread, (void*)&client_socket);

		if(thr_id<0){
			perror("Thread Create Error : ");
			exit(1);
		}
		
	}
	//소켓 닫기
	close(server_socket);
	
	printf("Server Closed\n");

	return 0;
}

//스레드에서 실행할 함수
void* socket_thread(void* para){
	int client_socket;
	char buf_recv[BUF_SIZE];
	char buf_snd[BUF_SIZE];
	int temp;
	client_socket = *((int*)para);
	
	while(temp = recv(client_socket, buf_recv, BUF_SIZE, 0)){//클라이언트로부터 메시지 받기

		if(strcmp(buf_recv,"q")==0) break;
		printf("Received By Client-%d : %s\n",client_socket, buf_recv);
		strcpy(buf_snd, buf_recv); //buf_snd에 buf_recv값 받기;
		
		send(client_socket, buf_snd, BUF_SIZE, 0); //클라이언트에게 받은 메시지 다시 클라이언트에게 보내기
		
	}
	printf("Client %d disconnected\n", client_socket);
	//스레드 종료
	pthread_exit(NULL);
}








