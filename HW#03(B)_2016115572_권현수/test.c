//test.c

#include <stdio.h> //printf
#include <stdlib.h> //atoi, exit
#include <pthread.h> //pthread
#include <unistd.h> //sleep, usleep

volatile int x; //��������
int flag_mutex = 0; //(new)���ؽ��� �� ���� ���� ���� ���� �����ϴ� ����
pthread_mutex_t mutex_lock; //���ؽ� ��ü ����

//////////Race Condition�� �����ִ� �Լ�!//////////
int RaceFunc(int x){
	int i, temp;
	for(i=0;i<50000;i++){
		x++;
		x--;
	}
	return x;
}

//add �Լ�
void* add(void* data){
	int i; //�ݺ�����
	int LOOP = *((int*)data); //void* �������� ���� �����͸� int�ڷ������� �ٲ۴�.
	
	if(flag_mutex == 1) pthread_mutex_lock(&mutex_lock); //�ٸ� ������ ������ ���´�.
	
	//critical section
	for(i=0;i<LOOP;i++){
		x++; // x ���� 1 ���Ѵ�.
		x = RaceFunc(x); //Critical section�� �Լ��� �߰��Ѵ�.
	}
	if(flag_mutex == 1) pthread_mutex_unlock(&mutex_lock); //�ٸ� �����尡 ����� �� �ְ� �Ѵ�.
	pthread_exit(NULL);
}


//sub �Լ�
void* sub(void* data){
	int i; //�ݺ�����
	int LOOP = *((int*)data); //void* �������� ���� �����͸� int�ڷ������� �ٲ۴�.

	if(flag_mutex == 1) pthread_mutex_lock(&mutex_lock); //�ٸ� ������ ������ ���´�.
	for(i=0;i<LOOP;i++){
		x--; //x ���� 1 ����.
		x = RaceFunc(x); //Critical section�� �Լ��� �߰��Ѵ�.
	}
	if(flag_mutex == 1) pthread_mutex_unlock(&mutex_lock); //�ٸ� �����尡 ����� �� �ְ� �Ѵ�.
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	pthread_t p_thread_add, p_thread_sub;
	//pthread_t �ڷ����� ������ ���� ����(add�Լ��� ������ ������ p_thread_add, sub�Լ��� ������ ������ p_thread_sub)
	int thr_id; //pthread_create�Լ��� return���� �޴� ����
	int i, j, n, _x, loop=1;
	 //i, j, n: �ݺ����� ���� ����, _x : �ʱⰪ�� �����ϱ� ���� ����, loop : add, sub�Լ� ������ �󸶳� �ݺ��� �������� ���ϴ� ����
	int Thread_Num;
	
	//������ �������� ������ �����ϴ� ����
	pthread_mutex_init(&mutex_lock, NULL); //���ؽ� ��ü �ʱ�ȭ, �⺻Ư������ �ʱ�ȭ��.
	if(argc != 3){ //(new - 2->3)������Ʈ�κ��� �޾ƿ� ���ڰ� 1���� �ƴҰ�� �Ʒ��ǿ����޽����� ����ϰ� ���α׷������մϴ�.
		printf("Type : ./test [argument] [num of Thread]\n"); //(new)
		exit(-1);
	}
	
	x = (atoi)(argv[1]); //���������� �� ��ȯ�� ���Ͽ� �ʱ�ȭ�� ���ݴϴ�.
	_x = x;
	Thread_Num = (atoi)(argv[2]);
		
	system("clear"); //����ϰ� ���� ���� ������Ʈ ���� ��� ����ϴ�.
	
	printf("Initial value : %d\n", x); //�ʱⰪ�� ����մϴ�.
	printf("Num of Thread : %d\n", Thread_Num); //(new)������ �������� ������ ����մϴ�.
	for(n=0;n<2;n++){
		if(flag_mutex == 0) printf("\n\t LOOP START - Mutex Disabled!\t\n\n"); //������ ������ �˸��ϴ�.
		else printf("\n\t LOOP START - Mutex Enabled!\t\n\n");
		for(j=1;j<=30;j++){ //30�� �ùķ��̼��� �ϱ� ���� �ݺ����� ���ϴ�.
			for(i=0;i<Thread_Num;i++){ //Thread_Num = 5000 ��ŭ �� �����带 �����մϴ�.
				//add ������ �����մϴ�.
				thr_id = pthread_create(&p_thread_add, NULL, add, (void*)&loop);
				if(thr_id<0){ //������ ���� ������, �Ʒ� ������ �����մϴ�.
					perror("Thread create error : ");
					exit(-1);
				}
				//sub ������ �����մϴ�.
				thr_id = pthread_create(&p_thread_sub, NULL, sub, (void*)&loop);
				if(thr_id<0){//������ ���� ������, �Ʒ� ������ �����մϴ�.
					perror("Thread create error : ");
					exit(-1);
				}
				
				//Add �����尡 ����Ǳ⸦ ��ٸ��ϴ�.
				pthread_join(p_thread_add,NULL);
				//Add �����尡 ����Ǳ⸦ ��ٸ��ϴ�.
				pthread_join(p_thread_sub,NULL);
			}
			//�ѹ��� �ùķ��̼��� ������, x�� ���� ���� �����ݴϴ�.
			if(x == _x){
				printf("Final Value x : %d\tSame!\t\t--%d\n",x,j); //�ʱⰪ�� �ùķ��̼��� ���� ���� ���� ���, Same!�� �Բ� ����մϴ�.
			}else{
				printf("Final Value x : %d\t\x1b[31mDifferent!\x1b[0m\t--%d\n",x,j); //�ݴ�� �ٸ���� Different!�� ����մϴ�. 
			}
			x = _x; //�ٲ�� ���� �� �ִ� �������� x ���� ���� �ʱⰪ���� �ٽ� �ǵ��� ���Ƽ� �ٽ� �ùķ��̼� �� �� �ְ� �մϴ�.
		}
		flag_mutex = 1;
	}
	pthread_mutex_destroy(&mutex_lock); //���ؽ� �ı�
	printf("Exit program\n"); //���α׷� ���Ḧ �˸��ϴ�.
	
	return 0;
}
