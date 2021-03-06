//구조체 배열을 이용  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 1000

//Student 구조체 정의  
typedef struct _Student{
	char name[20];
	char number[10];
}Student;

//Student를 CreateStudent함수를 통해 만든다. 
Student* CreateStudent(Student *st, char* name, char* number){
	st = (Student*)malloc(sizeof(Student));  
	if(st == NULL){
		printf("Student not created");
		return NULL;
	}
	
	//파라미터로 받은 name과 number를 각각 st의 name과 number에 넣는다. 이후 st반환. 
	strcpy(st->name, name); 
	strcpy(st->number, number);
	
	return st;
} 

int main(void){
	int i, number = 0;
	char* search = (char*)malloc(sizeof(char)*100); //검색변수 
	Student *student[NUM]; //student배열의 각각을 포인터로하여 Num만큼 만든다. 
	
	printf("Start Array Program\n\n");

	//아래 10줄은 각 student 배열에 내용을 넣는것. 
	student[0] = CreateStudent(student[0], "KwonHyeonSu", "2016115572");
	student[1] = CreateStudent(student[1], "KimHyeonSu", "5252525252");
	student[2] = CreateStudent(student[2], "ParkHyeonSu", "4343434343");
	student[3] = CreateStudent(student[3], "GuakHyeonSu", "7676767676");
	student[4] = CreateStudent(student[4], "JeongHYeonSu", "6464646464");
	student[5] = CreateStudent(student[5], "KwonMinSu", "5353535353");
	student[6] = CreateStudent(student[6], "KimMinSu", "8484848484");
	student[7] = CreateStudent(student[7], "ParkMinSu", "8585858585");
	student[8] = CreateStudent(student[8], "GuakMinSu", "1212121212");
	student[9] = CreateStudent(student[9], "JeongMinSu", "4364765434");

	printf("\n0 : Exit Program\n");
	printf("\n -----LIst----- \n\n");
	for(i=0;i<NUM;i++){
		if(student[i] == NULL) break;
		printf("list %d : %s, %s\n", i, student[i]->name, student[i]->number); //현재 메모리에 들어가있는 student관한 자료를 print한다. 
		number++;
	}

	printf("\ntest number : %d\n", number);
	
	while(strcmp(search, "0") != 0){ //search 변수가 0일경우 빠져나온다. 
		
		printf("\n\nSearch : ");
		scanf("%s", search); //search변수 받는다. 
		printf("\n");
		
		if(strcmp(search, "0") != 0){ //search변수가 0이 아닐 경우 아래 실행. 
			for(i=0;i<NUM;i++){
				if(student[i] == NULL) break;
				if(strcmp(student[i]->name, search) == 0){ //i번째의 student배열의 이름이 search와 같을 경우, i번째의 student의 학번을 프린트한다. 
					printf("-> %s", student[i]->number);
					break;
				}
				if(strcmp(student[i]->number, search) == 0){//i번째의 student배열의 학번이 search와 같을 경우, i번째의 student의 이름을 프린트한다.
					printf("-> %s", student[i]->name);
					break;
				}
				if(i==number-1) printf("\nnot Found!\n"); //student의 모든 배열을 돌려도 해당 값을 못찾을 경우에 프린트한다. 
			}
			
		} 
		
	}
	for(i=0;i<NUM;i++){
		if(student[i] != NULL) break;
		free(student[i]); //각 student 배열의 메모리를 해제해준다. 
	}
	free(search); //search변수 또한 해제해준다. 
	printf("memory deallocated, Exit program\n\n");
	
	return 0;
}
