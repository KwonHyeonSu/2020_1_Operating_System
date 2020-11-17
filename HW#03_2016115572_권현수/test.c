//test.c

#include <stdio.h> //printf
#include <stdlib.h> //atoi, exit
#include <pthread.h> //pthread
#include <unistd.h> //sleep, usleep

#define Thread_Num 5000 //스레드 개수 5000개

volatile int x; //전역변수

//add 함수
void* add(void* data){
	int i; //반복문용
	int LOOP = *((int*)data); //void* 형식으로 들어온 데이터를 int자료형으로 바꾼다.
	for(i=0;i<LOOP;i++){
		x++; // x 값에 1 더한다.
		//printf("ADD -> x : %d\t----- %d\n",x, i); //add 함수로 인해 하나씩 더해질때마다 x값을 출력한다. 디버깅용.
	}
	pthread_exit(NULL);
}


//sub 함수
void* sub(void* data){
	int i; //반복문용
	int LOOP = *((int*)data); //void* 형식으로 들어온 테이터를 int자료형으로 바꾼다.
	for(i=0;i<LOOP;i++){
		x--; //x 값에 1 뺀다.
		//printf("SUB -> x : %d\t----- %d\n",x, i); //sub 함수로 인해 하나씩 빼질때마다 x값을 출력한다. 디버깅용.
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	pthread_t p_thread_add, p_thread_sub;
	//pthread_t 자료형의 쓰레드 변수 생성(add함수를 실행할 쓰레드 p_thread_add, sub함수를 실행할 쓰레드 p_thread_sub)
	int thr_id; //pthread_create함수의 return값을 받는 변수
	int i, j, n=1, _x, loop=1;
	 //i, j, n: 반복문을 위한 변수, _x : 초기값을 저장하기 위한 변수, loop : add, sub함수 내에서 얼마나 반복할 것인지를 정하는 변수
	
	if(argc != 2){ //프롬프트로부터 받아온 인자가 1개가 아닐경우 아래의오류메시지를 출력하고 프로그램종료합니다.
		printf("you must type only one argument!\n");
		exit(-1);
	}
	
	x = (atoi)(argv[1]); //전역변수를 형 변환을 통하여 초기화를 해줍니다.
	_x = x;
	
	system("clear"); //깔끔하게 보기 위해 프롬프트 내용 모두 지웁니다.
	
	printf("Initial value : %d\n",x); //초기값을 출력합니다.
	while(n<=3){ //loop가 10, 100, 1000일 경우 총 3번 돌게 합니다.
		loop *= 10; //loop의 초기값 1에서, 반복문 한번이 돌 때마다 10이 곱해져서 10, 100, 1000이 됩니다.
		printf("\n\t%d LOOP START\t\n\n", loop); //루프의 시작을 알립니다.
		for(j=1;j<=30;j++){ //30번 시뮬레이션을 하기 위해 반복문을 씁니다.
			for(i=0;i<Thread_Num;i++){ //Thread_Num = 5000 만큼 각 스레드를 생성합니다.
				//add 스레드 생성합니다.
				thr_id = pthread_create(&p_thread_add, NULL, add, (void*)&loop);
				if(thr_id<0){ //쓰레드 생성 오류시, 아래 내용을 실행합니다.
					perror("Thread create error : ");
					exit(-1);
				}
				//sub 스레드 생성합니다.
				thr_id = pthread_create(&p_thread_sub, NULL, sub, (void*)&loop);
				if(thr_id<0){//쓰레드 생성 오류시, 아래 내용을 실행합니다.
					perror("Thread create error : ");
					exit(-1);
				}
				
				//Add 스레드가 종료되기를 기다립니다.
				pthread_join(p_thread_add,NULL);
				//Add 스레드가 종료되기를 기다립니다.
				pthread_join(p_thread_sub,NULL);
			}
			//한번의 시뮬레이션이 끝나고, x의 최종 값을 보여줍니다.
			if(x == _x){
				printf("Final Value x : %d\tSame!\t\t--%d\n",x,j); //초기값과 시뮬레이션이 끝난 값이 같을 경우, Same!을 함께 출력합니다.
			}else{
				printf("Final Value x : %d\t\x1b[31mDifferent!\x1b[0m\t--%d\n",x,j); //반대로 다를경우 Different!를 출력합니다. 
			}
			x = _x; //바뀌어 있을 수 있는 전역변수 x 값을 원래 초기값으로 다시 되돌려 놓아서 다시 시뮬레이션 할 수 있게 합니다.
		}
		n++; //n이 3이 넘게되면 while문이 끝납니다.
	}
	printf("Exit program\n"); //프로그램 종료를 알립니다.
	
	return 0;
}
