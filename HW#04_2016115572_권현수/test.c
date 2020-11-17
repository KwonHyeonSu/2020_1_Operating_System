#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

//스택 주소정보를 저장할 메시지큐 구조체
struct msgbuf{
	long mtype;
	unsigned long data;
};

//힙 주소정보를 저장할 메시지큐 구조체
struct msgbuf2{
	long mtype;
	unsigned long data;
};

int main(){

	pid_t pid_stack, pid_heap; //스택과 힙의 프로세스 아이디를 저장할 변수
	int msgid, msgid2; //msgid : 스택이 사용할 메시지큐 아이디, msgid2 : 힙이 사용할 메시지큐 아이디
	int status, status2; //waitpid()함수에서 쓸 인자, status는 스택프로세스의 상태, status2는 힙프로세스의 상태
	int option; //waitpid()함수에서 쓸 인자
	struct msgbuf mbuf; //스택에서 사용할 메시지큐 구조체 변수
	struct msgbuf2 mbuf2; //힙에서 사용할 메시지큐 구조체 변수
	unsigned long stackLow=0; //스택의 맨 아래 주소를 저장할 변수
	unsigned long heapHigh=0;//힙의 맨 아래 주소를 저장할 변수
	char arg1[100000]; //힙에 넣어줄 인자(할당할 메가바이트 크기)
	
	
	//힙에 넣어줄 메가바이트 크기를 미리 받음
	printf("Enter heap argument <MemorySizeInMegabyte> : ");
	if(-1 == scanf("%s", arg1)){
		perror("scanf failed\n");
		exit(1);
	}
	
	if(atoi(arg1)<0){
		printf("arg1 must be positive!\n");
		exit(1);
	}
	
	//스택 메시지큐 생성
	msgid = msgget((key_t)16, IPC_CREAT|0666);
	if(msgid<0){
		printf("msgget failed\n");
		exit(1);
	}
	
	//프로세스 생성
	pid_stack = fork();
	if(pid_stack<0){
		printf("fork failed\n");
		exit(1);
	}
	
	
	//생성된 자식 프로세스를 execlp함수를 통해 stack 프로세스 실행
	else if(pid_stack==0){
		execlp("./stack", "./stack", NULL);
		printf("execlp failed!\n");
		exit(1);
	}
	
	else{
		pid_heap = fork(); //새로운 프로세스 하나 더 생성
		if(pid_heap<0){
			perror("fork failed\n");
			exit(1);
		}
		//생성된 새로운 자식 프로세스를 execlp함수를 통해 heap 프로세스 실행
		else if(pid_heap == 0){
			execlp("./heap", "./heap", arg1, "Nothing", NULL); //기본값 Nothing으로 인자가 넘어가게 해놓음.
			printf("execlp failed!\n");
			exit(1);
		}
		else{
			waitpid(pid_stack, &status, 0); //스택프로세스 기다림
			waitpid(pid_heap, &status2, 0); //힙프로세스 기다림


			sleep(1); //모두 실행이 종료 되고, 사용자의 보기 편함을 위해 1초 쉼
			
			while(1){
				//스택의 경우, 메시지큐 범위를 넘는 경우가 없었음. 그래서 반복문을 통해 메시지 큐 안의 가장 작은 값을 찾아냄
				if(-1 == msgrcv(msgid, (void*)&mbuf, sizeof(mbuf)-sizeof(long), 1, IPC_NOWAIT)){
					
					break;
				}
				if(stackLow == 0) stackLow = mbuf.data;
				//printf("stackLow : %ld\n", stackLow);
				if(mbuf.data<stackLow) stackLow = mbuf.data;
			}
		
			//코드 구조상 heap실행중에 메시지큐가 삭제되므로 여기서 새로 만듬
			msgid2 = msgget((key_t)32, IPC_CREAT|0666);
			if(msgid<0){
				printf("msgget failed\n");
				exit(1);
			}
			
			//메시지를 받아와서 힙 할당영역에서의 가장 높은 주소를 가져옴
			if(-1 == msgrcv(msgid2, (void*)&mbuf2, sizeof(mbuf2)-sizeof(long), 1, IPC_NOWAIT)){
					perror("There is no more Message	-heap\n");
			}

			heapHigh = mbuf2.data; //가장 높은 주소를 heapHigh에 저장.
			
			printf("\n\n");
			printf("stackLow :	 %lx\n",stackLow);
			printf("heapHigh :	 %lx\n",heapHigh);
			
			if(WIFSIGNALED(status2)!=0 && WIFSIGNALED(status)!=0){ //정상적으로 종료되지 않았을 경우
				//저장해놓은 스택의 가장 낮은 주소와 힙의 가장 높은 주소사이에서 충돌이 발생함을 알려줌.
				printf("\n\x1b[31mCrashed between	%lx ~ %lx\n\n", stackLow, heapHigh); 
			}
			else{//정상적으로 종료되었을 경우
				printf("\n\n\x1b[34mCrash not occured.\n\n");
			}
		}
	}
	
	return 0;
}
