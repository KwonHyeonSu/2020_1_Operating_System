#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //���� ���
#include <netinet/in.h> //���� ���
#include <unistd.h>

#define PORT 7777
#define BUF_SIZE 1024

int main(int argc, char** argv){ //argv[1]�� ��Ʈ��ȣ ���� �� ����.
	
	int client_socket;
	
	char buf_snd[BUF_SIZE]; //���� ���ڿ�����
	char buf_recv[BUF_SIZE]; //���� ���ڿ� ����
	
	struct sockaddr_in server_addr;

	//Ŭ���̾�Ʈ ���� ����
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(client_socket == -1){
		perror("Socket Create Error ");
		exit(1);
	}
	printf("Socket Create Complete...\n");
	
	memset(&server_addr, 0, sizeof(server_addr));
	//�������� ����ü�� ��ġ��Ų��.

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	
	if(argc == 1) server_addr.sin_port = htons(PORT);
	else server_addr.sin_port = htons((atoi)(argv[1]));
	
	//���� ���Ͽ� ���� ��û
	if(-1==connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
		perror("Connect error ");
		exit(1);
	}
	printf("Connect Complete...\n");
	
	
	
	//���������� recv, send�� �� �ְ� ���� �ݺ�.
	while(1){
		printf("Enter Your Message (q to exit) : ");
		fgets(buf_snd, BUF_SIZE, stdin); // �Է��� �޾Ƽ� buf_snd�� ����
		buf_snd[strlen(buf_snd)-1] = '\0'; //����Ű ����
		if(strcmp(buf_snd, "q")==0) break;
		send(client_socket, buf_snd, BUF_SIZE, 0); //buf_snd�� ������ ����
		
		recv(client_socket, buf_recv, BUF_SIZE, 0); //���� �޽����� buf_recv�� ����
		printf("Received %s\n", buf_recv); 
		
		
	}
	send(client_socket, "q",BUF_SIZE,0);
	printf("Client close\n");
	//Ŭ���̾�Ʈ ���� �ݱ�;
	close(client_socket);
	
}

