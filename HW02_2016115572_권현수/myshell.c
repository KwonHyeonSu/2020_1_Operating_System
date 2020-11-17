#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork(), execvp()
#include <wait.h> //wait()
#include <string.h> //strcmp(), strtok(), strlen()
#define MAX 255 //define MAX to 255

int main(){
	//pid => fork()�Լ��� ���ϰ��� ��ȯ�Ѵ�.
	//_exec => �ڽ� �Լ��� ã�� �����μ�, ��ã���� ��� -1�� ��ȯ���� �����ϱ� ����.
	//count => �� _input���� ���� ���� value ���� ��, value[0]���� ���ʴ�� ���� ����.
	int pid=0, _exec, order=0, i; 
	

	//value => execvp�� ���ڷμ� �ֱ� ���� _value�� strtok�Լ��� Ȱ���� ���� ���� �ϳ��ϳ� ������.
	//_value => ����Ű�� ������ ��� �Է� ���� �� ���� �����ϱ� ����.
	//temp => strtok�� ����� ��, _value���� value �ֱ� ���� �ӽ÷� ����� �� ����.
	char* value[100]; 
	char _value[30];
	char* temp;
	char hBuf[MAX], uBuf[MAX], dBuf[MAX];
	
	gethostname(hBuf, MAX); //hostname�� ����
	getlogin_r(uBuf, MAX); //username�� ����
	getcwd(dBuf, MAX); //directoryname�� ����

	//���α׷� ���� ���.
	printf("\n-----How to Use-----\n");
	printf("\n0 : Quit\n\n");
	printf("sum -> ./sum arg1 arg2\n");
	printf("sub -> ./sub arg1 arg2\n");
	printf("fact -> ./fact arg1\n");
	printf("div -> ./div arg1 arg2\n");
	printf("\n-----How to Use-----\n");
	
	//myshell�� �ݺ��Ͽ� �����ϱ� ���� �ݺ���.
	while(1){
		printf("\n%s@%s:~%s$ ",uBuf, hBuf, dBuf); //���� ������Ʈ�� ���� ������ ����ϰ� ����Ʈ�Ѵ�.
		
		fgets(_value,100,stdin);
		_value[strlen(_value)-1] = 0;
		//fgets�Լ��� �̿��Ͽ� ���� ��ü�� ���� ����, _value������ �ְ�, ������ �κ��� 0���� ���� ���ڿ��� ���� �˷��ش�.
		
		if(strcmp(_value,"0")==0) break; //"0"�� �Է����� ��, while���� ���������鼭 ���α׷� ����� �� �ְ� ���α׷��ߴ�..
		
		//�Ʒ� while������ �Ͽ��� value���� ���� �������� strtok()�Լ��� �̿��� �ܾ�� �����Ѵ�.
		temp = strtok(_value," ");

		while(temp != NULL){
			value[order] = temp;
			temp = strtok(NULL, " ");
			order++;
		}

		value[order] = NULL; //value �迭�� ���κ��� NULL�� �����Ͽ� execvp�Լ��� ���� ������ ������Ų��.

		pid = fork(); //�ڽ� ���α׷��� �ϳ� ����.
		
		//�ڽ��� �������� �ʾ��� ���(pid<0), Fork Failed�� ����ϰ� �ý��� ����.
		if(pid<0){
			fprintf(stderr, "\nFork Failed\n");
			exit(-1);	
		}
		
		//�ڽ����α׷��� �д� �κ����μ�, ���ϴ� �Լ��� �����Ѵ�. ���� ã�� ���ϸ�(execvp�� ���ϰ��� -1�̸�)Cannot find function!��� �Ŀ� �����޽����� ����ϰ�, �ڽ� ���α׷��� �����Ѵ�.
		else if(pid==0){
			_exec = execvp(value[0], value);
			if(_exec == -1){
				perror("\nCannot find function!");
			}
			exit(-1);
		}
		
		//�θ� ���α׷��� �д� �κ����μ�, �ڽ� ���α׷��� ���� �� ���� ����Ų��. (���α׷� �ߺ� ����)
		else{
			wait(NULL);
		}
		order = 0; //order�� 0���� ����� �����μ�, ���α׷��� �ݺ����� ���� ó������ ���ư��� �� input[0]���� ���ʴ�� ���� �Ѵ�.
	}

	//����ڰ� "0"�� �Է��Ͽ� �ݺ����� ���������� ��� ����Ǵ� �κ��̴�.
	printf("\nexit myshell Program\n\n");
	exit(0);
	return 0;
}

