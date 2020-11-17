#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork(), execvp()
#include <wait.h> //wait()
#include <string.h> //strcmp(), strtok(), strlen()
#define MAX 255 //define MAX to 255

int main(){
	//pid => fork()함수의 리턴값을 반환한다.
	//_exec => 자식 함수를 찾는 변수로서, 못찾았을 경우 -1의 반환값을 저장하기 위함.
	//count => 이 _input으로 받은 값을 value 넣을 때, value[0]부터 차례대로 들어가기 위함.
	int pid=0, _exec, order=0, i; 
	

	//value => execvp의 인자로서 넣기 위해 _value을 strtok함수를 활용해 나눈 값을 하나하나 저장함.
	//_value => 엔터키를 제외한 모든 입력 값을 한 번에 저장하기 위함.
	//temp => strtok를 사용할 때, _value값을 value 넣기 위해 임시로 만들어 둔 변수.
	char* value[100]; 
	char _value[30];
	char* temp;
	char hBuf[MAX], uBuf[MAX], dBuf[MAX];
	
	gethostname(hBuf, MAX); //hostname을 저장
	getlogin_r(uBuf, MAX); //username을 저장
	getcwd(dBuf, MAX); //directoryname을 저장

	//프로그램 사용법 명시.
	printf("\n-----How to Use-----\n");
	printf("\n0 : Quit\n\n");
	printf("sum -> ./sum arg1 arg2\n");
	printf("sub -> ./sub arg1 arg2\n");
	printf("fact -> ./fact arg1\n");
	printf("div -> ./div arg1 arg2\n");
	printf("\n-----How to Use-----\n");
	
	//myshell을 반복하여 실행하기 위한 반복문.
	while(1){
		printf("\n%s@%s:~%s$ ",uBuf, hBuf, dBuf); //실제 프롬프트와 같진 않지만 비슷하게 프린트한다.
		
		fgets(_value,100,stdin);
		_value[strlen(_value)-1] = 0;
		//fgets함수를 이용하여 한줄 전체를 받은 다음, _value변수에 넣고, 마지막 부분을 0으로 만들어서 문자열의 끝을 알려준다.
		
		if(strcmp(_value,"0")==0) break; //"0"을 입력했을 때, while문을 빠져나가면서 프로그램 종료될 수 있게 프로그램했다..
		
		//아래 while문까지 하여서 value값을 띄어쓰기 기준으로 strtok()함수를 이용해 단어별로 저장한다.
		temp = strtok(_value," ");

		while(temp != NULL){
			value[order] = temp;
			temp = strtok(NULL, " ");
			order++;
		}

		value[order] = NULL; //value 배열의 끝부분을 NULL로 설정하여 execvp함수의 실행 조건을 만족시킨다.

		pid = fork(); //자식 프로그램을 하나 생성.
		
		//자식이 생성되지 않았을 경우(pid<0), Fork Failed를 출력하고 시스템 종료.
		if(pid<0){
			fprintf(stderr, "\nFork Failed\n");
			exit(-1);	
		}
		
		//자식프로그램이 읽는 부분으로서, 원하는 함수를 실행한다. 만약 찾지 못하면(execvp의 리턴값이 -1이면)Cannot find function!출력 후에 에러메시지를 출력하고, 자식 프로그램을 종료한다.
		else if(pid==0){
			_exec = execvp(value[0], value);
			if(_exec == -1){
				perror("\nCannot find function!");
			}
			exit(-1);
		}
		
		//부모 프로그램이 읽는 부분으로서, 자식 프로그램이 끝날 때 까지 대기시킨다. (프로그램 중복 방지)
		else{
			wait(NULL);
		}
		order = 0; //order를 0으로 만들어 줌으로서, 프로그램이 반복문을 통해 처음으로 돌아갔을 때 input[0]부터 차례대로 들어가게 한다.
	}

	//사용자가 "0"을 입력하여 반복문을 빠져나왔을 경우 실행되는 부분이다.
	printf("\nexit myshell Program\n\n");
	exit(0);
	return 0;
}

