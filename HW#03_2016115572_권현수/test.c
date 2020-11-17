//test.c

#include <stdio.h> //printf
#include <stdlib.h> //atoi, exit
#include <pthread.h> //pthread
#include <unistd.h> //sleep, usleep

#define Thread_Num 5000 //������ ���� 5000��

volatile int x; //��������

//add �Լ�
void* add(void* data){
	int i; //�ݺ�����
	int LOOP = *((int*)data); //void* �������� ���� �����͸� int�ڷ������� �ٲ۴�.
	for(i=0;i<LOOP;i++){
		x++; // x ���� 1 ���Ѵ�.
		//printf("ADD -> x : %d\t----- %d\n",x, i); //add �Լ��� ���� �ϳ��� ������������ x���� ����Ѵ�. ������.
	}
	pthread_exit(NULL);
}


//sub �Լ�
void* sub(void* data){
	int i; //�ݺ�����
	int LOOP = *((int*)data); //void* �������� ���� �����͸� int�ڷ������� �ٲ۴�.
	for(i=0;i<LOOP;i++){
		x--; //x ���� 1 ����.
		//printf("SUB -> x : %d\t----- %d\n",x, i); //sub �Լ��� ���� �ϳ��� ���������� x���� ����Ѵ�. ������.
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	pthread_t p_thread_add, p_thread_sub;
	//pthread_t �ڷ����� ������ ���� ����(add�Լ��� ������ ������ p_thread_add, sub�Լ��� ������ ������ p_thread_sub)
	int thr_id; //pthread_create�Լ��� return���� �޴� ����
	int i, j, n=1, _x, loop=1;
	 //i, j, n: �ݺ����� ���� ����, _x : �ʱⰪ�� �����ϱ� ���� ����, loop : add, sub�Լ� ������ �󸶳� �ݺ��� �������� ���ϴ� ����
	
	if(argc != 2){ //������Ʈ�κ��� �޾ƿ� ���ڰ� 1���� �ƴҰ�� �Ʒ��ǿ����޽����� ����ϰ� ���α׷������մϴ�.
		printf("you must type only one argument!\n");
		exit(-1);
	}
	
	x = (atoi)(argv[1]); //���������� �� ��ȯ�� ���Ͽ� �ʱ�ȭ�� ���ݴϴ�.
	_x = x;
	
	system("clear"); //����ϰ� ���� ���� ������Ʈ ���� ��� ����ϴ�.
	
	printf("Initial value : %d\n",x); //�ʱⰪ�� ����մϴ�.
	while(n<=3){ //loop�� 10, 100, 1000�� ��� �� 3�� ���� �մϴ�.
		loop *= 10; //loop�� �ʱⰪ 1����, �ݺ��� �ѹ��� �� ������ 10�� �������� 10, 100, 1000�� �˴ϴ�.
		printf("\n\t%d LOOP START\t\n\n", loop); //������ ������ �˸��ϴ�.
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
		n++; //n�� 3�� �ѰԵǸ� while���� �����ϴ�.
	}
	printf("Exit program\n"); //���α׷� ���Ḧ �˸��ϴ�.
	
	return 0;
}
