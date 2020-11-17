#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //���� ���
#include <netinet/in.h> //���� ���
#include <pthread.h> //������ ���
#include <unistd.h>

#define PORT 7777 //��Ʈ��ȣ 7777
#define BUF_SIZE 1024 //BUF_SIZE = 1024

void* socket_thread(void* para);
int main(int argc, char** argv){ //argv[1]�� ��Ʈ��ȣ ���� �� ����.
	
	int server_socket; //���� ���� ��ũ���� ����
	int client_socket; //Ŭ���̾�Ʈ ���� ��ũ���� ����
	int client_addr_size; //client_addr�� ũ�� ����
	int n=0; //������ Ŭ���̾�Ʈ ����
	int option; //bind���� �ذ��ϱ� ���� ����
	pthread_t thr_id; //������ ����
	
	struct sockaddr_in server_addr; //sockaddr_in ���� ����ü
	struct sockaddr_in client_addr; //sockaddr_in Ŭ���̾�Ʈ ����ü

	//PF_INET : TCP/IP��� , SOCK_STREAM : TCP���, 0 : Ÿ�Կ��� �̸� �������� ��.
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	option = 1; //SO_REUSEADDR �� �ɼǰ��� TRUE�� �ٲ���
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	
	if(server_socket == -1){
		perror("Socket Create Error : ");
		exit(1);
	}
	printf("Socket Create Complete...\n");

	//server_addr ����ü ����
	server_addr.sin_family = AF_INET; //AF_INET : IP �ּ� ü�� ��Ī
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY : IP �ּҰ� �ڵ����� �Ҵ�
	
	if(argc == 1) server_addr.sin_port = htons(PORT);//htons : PORT(=5555)�� HOST(��Ʋ�����)���Ŀ��� NETWORK(�򿣵��)�������� ��ȯ
	else server_addr.sin_port = htons((atoi)(argv[1]));

	if(-1==bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
		perror("Bind error : ");
		exit(1);
	}
	printf("Bind Complete...\n");
	
	//Ŭ���̾�Ʈ�� ��� ���� �� �ְ� ���ѷ����� ����
	while(1){

	 	//client�� ��û�� ��ٸ�.
		if(-1==listen(server_socket, 3)){
			perror("Listen error : ");
			exit(1);
		}
		printf("Listening...\n");
		
		client_addr_size = sizeof(client_addr);
		//��û�� ������.
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
		if(client_socket == -1){
			perror("Accept error : ");
			exit(1);
		}
		printf("Accept Complete...%d\n", ++n);
		
		//�����带 �����Ͽ� send, recv�Լ� ����.
		pthread_create(&thr_id, NULL, socket_thread, (void*)&client_socket);

		if(thr_id<0){
			perror("Thread Create Error : ");
			exit(1);
		}
		
	}
	//���� �ݱ�
	close(server_socket);
	
	printf("Server Closed\n");

	return 0;
}

//�����忡�� ������ �Լ�
void* socket_thread(void* para){
	int client_socket;
	char buf_recv[BUF_SIZE];
	char buf_snd[BUF_SIZE];
	int temp;
	client_socket = *((int*)para);
	
	while(temp = recv(client_socket, buf_recv, BUF_SIZE, 0)){//Ŭ���̾�Ʈ�κ��� �޽��� �ޱ�

		if(strcmp(buf_recv,"q")==0) break;
		printf("Received By Client-%d : %s\n",client_socket, buf_recv);
		strcpy(buf_snd, buf_recv); //buf_snd�� buf_recv�� �ޱ�;
		
		send(client_socket, buf_snd, BUF_SIZE, 0); //Ŭ���̾�Ʈ���� ���� �޽��� �ٽ� Ŭ���̾�Ʈ���� ������
		
	}
	printf("Client %d disconnected\n", client_socket);
	//������ ����
	pthread_exit(NULL);
}








