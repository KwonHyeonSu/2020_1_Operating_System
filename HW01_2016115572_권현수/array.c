//����ü �迭�� �̿�  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 1000

//Student ����ü ����  
typedef struct _Student{
	char name[20];
	char number[10];
}Student;

//Student�� CreateStudent�Լ��� ���� �����. 
Student* CreateStudent(Student *st, char* name, char* number){
	st = (Student*)malloc(sizeof(Student));  
	if(st == NULL){
		printf("Student not created");
		return NULL;
	}
	
	//�Ķ���ͷ� ���� name�� number�� ���� st�� name�� number�� �ִ´�. ���� st��ȯ. 
	strcpy(st->name, name); 
	strcpy(st->number, number);
	
	return st;
} 

int main(void){
	int i, number = 0;
	char* search = (char*)malloc(sizeof(char)*100); //�˻����� 
	Student *student[NUM]; //student�迭�� ������ �����ͷ��Ͽ� Num��ŭ �����. 
	
	printf("Start Array Program\n\n");

	//�Ʒ� 10���� �� student �迭�� ������ �ִ°�. 
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
		printf("list %d : %s, %s\n", i, student[i]->name, student[i]->number); //���� �޸𸮿� ���ִ� student���� �ڷḦ print�Ѵ�. 
		number++;
	}

	printf("\ntest number : %d\n", number);
	
	while(strcmp(search, "0") != 0){ //search ������ 0�ϰ�� �������´�. 
		
		printf("\n\nSearch : ");
		scanf("%s", search); //search���� �޴´�. 
		printf("\n");
		
		if(strcmp(search, "0") != 0){ //search������ 0�� �ƴ� ��� �Ʒ� ����. 
			for(i=0;i<NUM;i++){
				if(student[i] == NULL) break;
				if(strcmp(student[i]->name, search) == 0){ //i��°�� student�迭�� �̸��� search�� ���� ���, i��°�� student�� �й��� ����Ʈ�Ѵ�. 
					printf("-> %s", student[i]->number);
					break;
				}
				if(strcmp(student[i]->number, search) == 0){//i��°�� student�迭�� �й��� search�� ���� ���, i��°�� student�� �̸��� ����Ʈ�Ѵ�.
					printf("-> %s", student[i]->name);
					break;
				}
				if(i==number-1) printf("\nnot Found!\n"); //student�� ��� �迭�� ������ �ش� ���� ��ã�� ��쿡 ����Ʈ�Ѵ�. 
			}
			
		} 
		
	}
	for(i=0;i<NUM;i++){
		if(student[i] != NULL) break;
		free(student[i]); //�� student �迭�� �޸𸮸� �������ش�. 
	}
	free(search); //search���� ���� �������ش�. 
	printf("memory deallocated, Exit program\n\n");
	
	return 0;
}
