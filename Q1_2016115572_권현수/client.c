#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //소켓 헤더
#include <netinet/in.h> //소켓 헤더
#include <unistd.h>

#define PORT 7777
#define BUF_SIZE 1024

int main(int argc, char** argv){ //argv[1]로 포트번호 정할 수 있음.
	
	int client_socket;
	
	char buf_snd[BUF_SIZE]; //보낼 문자열변수
	char buf_recv[BUF_SIZE]; //받을 문자열 변수
	
	struct sockaddr_in server_addr;

	//클라이언트 소켓 생성
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(client_socket == -1){
		perror("Socket Create Error ");
		exit(1);
	}
	printf("Socket Create Complete...\n");
	
	memset(&server_addr, 0, sizeof(server_addr));
	//서버쪽의 구조체와 일치시킨다.

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	
	if(argc == 1) server_addr.sin_port = htons(PORT);
	else server_addr.sin_port = htons((atoi)(argv[1]));
	
	//서버 소켓에 연결 요청
	if(-1==connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
		perror("Connect error ");
		exit(1);
	}
	printf("Connect Complete...\n");
	
	
	
	//지속적으로 recv, send할 수 있게 무한 반복.
	while(1){
		printf("Enter Your Message (q to exit) : ");
		fgets(buf_snd, BUF_SIZE, stdin); // 입력을 받아서 buf_snd에 저장
		buf_snd[strlen(buf_snd)-1] = '\0'; //엔터키 빼기
		if(strcmp(buf_snd, "q")==0) break;
		send(client_socket, buf_snd, BUF_SIZE, 0); //buf_snd를 서버에 보냄
		
		recv(client_socket, buf_recv, BUF_SIZE, 0); //받은 메시지를 buf_recv에 저장
		printf("Received %s\n", buf_recv); 
		
		
	}
	send(client_socket, "q",BUF_SIZE,0);
	printf("Client close\n");
	//클라이언트 소켓 닫기;
	close(client_socket);
	
}

