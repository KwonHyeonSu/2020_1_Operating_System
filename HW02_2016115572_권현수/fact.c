//fact

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//fact함수(재귀함수)
int fact(int a){
	if(a==1) return 1;
	return a*fact(a-1);
}

int main(int argc, char* argv[]){
	int re=0;

	//파라미터 1개와 fact를 포함한 총 인자 갯수가 2개가 아닐 경우 1개의 인자가 필요하다고 명시해준다.
	if(argc != 2){
		printf("\nNeed 1 argument!\n");
		return 0;
	}

	//인자값이 0일 경우, 1을 출력하고, 0보다 작을경우 오류메시지를 출한다.
	if((atoi)(argv[1])==0) printf("\nfact(%d) = 1\n",(atoi)(argv[1]));
	else if((atoi)(argv[1])<0) printf("\nargument must be 0 or positive number!\n");
	else{
		re = fact((atoi)(argv[1]));
		printf("\nfact(%d) = %d\n",(atoi)(argv[1]), re);
	}

	return 0;
}
