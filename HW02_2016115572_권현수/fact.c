//fact

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//fact�Լ�(����Լ�)
int fact(int a){
	if(a==1) return 1;
	return a*fact(a-1);
}

int main(int argc, char* argv[]){
	int re=0;

	//�Ķ���� 1���� fact�� ������ �� ���� ������ 2���� �ƴ� ��� 1���� ���ڰ� �ʿ��ϴٰ� ������ش�.
	if(argc != 2){
		printf("\nNeed 1 argument!\n");
		return 0;
	}

	//���ڰ��� 0�� ���, 1�� ����ϰ�, 0���� ������� �����޽����� ���Ѵ�.
	if((atoi)(argv[1])==0) printf("\nfact(%d) = 1\n",(atoi)(argv[1]));
	else if((atoi)(argv[1])<0) printf("\nargument must be 0 or positive number!\n");
	else{
		re = fact((atoi)(argv[1]));
		printf("\nfact(%d) = %d\n",(atoi)(argv[1]), re);
	}

	return 0;
}
