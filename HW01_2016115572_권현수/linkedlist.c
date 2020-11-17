//Linkedlist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
//student ����ü ���� 
typedef struct _Student{
	char name[20];
	char number[10];
}Student;
 
//student��� ����ü�� ���� ��� ���� 
typedef struct _Node{
	Student *data;
	struct _Node *next;
}Node;

//��带 ���� ����Ʈ ���� - ������� cur, head, tail 
typedef struct _list{
	Node *cur, *head, *tail;
}List;

//main���� ���� list��, main���� ������ name �� number�� �̿��Ͽ� list�ȿ� student�� ����
//name�� number�� �ִ´�. 
void CreateStudent(List* list, char* name, char* number){
	
	//���θ��� ���� student�� �����Ҵ����ش�. 
	Node *newNode = (Node*)malloc(sizeof(Node));
	if(newNode == NULL) return;
	
	Student *newStudent = (Student*)malloc(sizeof(Student));
	if(newStudent == NULL) return;
	
	strcpy(newStudent->name, name);
	strcpy(newStudent->number, number);
	
	//���ο� ��忡 ������ ����ִ´�. 
	newNode->data = newStudent;
	newNode->next = NULL;
	
	//list�� �ƹ��͵� ���� ���, list�� head�� tail�� ���ο� ���� �ִ´�. 
	if(list->head == NULL && list->tail == NULL) list->head = list->tail = newNode;
	else{//�׷��� ���� ���, �����ʿ� ���ο� ��带 �ִ´�. 
		list->tail->next = newNode;
		list->tail = newNode;
	}
}

//��带 �ı��ϴ� �Լ�  
void Destroylist(List* list){
	//���� ���� �� ���� ã�ư��� ��� 
	Node *delNode = list->head;
	Node *NextNode = delNode->next;
	
	
	while(NextNode!=NULL){ //nextnode�� null�� ��� ��ž 
		
		delNode->data = NULL;
		free(delNode->data); //node�ȿ��ִ� data ��, student�� �Ҵ������Ѵ�. 
		
		free(delNode); //��� ��ü�� �Ҵ������Ѵ�. 
		delNode = NextNode;
		NextNode = NextNode->next; //nextnode�� �������� �̵��Ͽ� ��� ��带 ã�� �Ѵ�. 
	}
	free(delNode);
	free(NextNode); 
}

//list�ȿ��ִ� ��� ��带 �˻��Ͽ� ��� �ȿ��ִ� student(data)�� name�� number�� ����Ѵ�. 
void Nprint(List* list){
	Node *Ntemp = list->head;
	while(Ntemp != NULL){
		printf("%s, %s\n",Ntemp->data->name, Ntemp->data->number);
		Ntemp = Ntemp->next;
	}
}

//list�ȿ��ִ� ����� student�� name�� number�� search���� �������� �ִ��� Ȯ���ϰ� �Ѵ�. 
void Nsearch(List* list, char* search){
	Node *Ntemp = list->head;
	while(Ntemp!= NULL){
		if(strcmp(search, Ntemp->data->name) == 0){
			printf("\n->%s\n\n",Ntemp->data->number);
			break;
		}else if(strcmp(search, Ntemp->data->number) == 0){
			printf("\n->%s\n\n",Ntemp->data->name);
			break;
		}
		Ntemp = Ntemp->next;
		if(Ntemp == NULL) printf("\nnot Found!\n\n");
	}
}

int main(void){
	
	int i;
	char *search = (char*)malloc(sizeof(char)*100); //ã�����ϴ� ���� ���� ����. �����Ҵ���. 
	
	//list�� �������ְ�, cur, head, tail���� null�� �ʱ�ȭ ���ش�. 
	List *list = (List*)malloc(sizeof(List));
	list->cur = NULL;
	list->head = NULL;
	list->tail = NULL;
	
	printf("linkedlist\n\n");
	
	//�ʱ�ȭ�� list�� ���� �̸��� �й��� �ִ´�. 
	CreateStudent(list, "KwonHyeonSu", "2016115572");
	CreateStudent(list, "KooHyeonSu", "2015215432");
	CreateStudent(list, "KimHyeonSu", "5252525252");
	CreateStudent(list, "ParkHyeonSu", "4343434343");
	CreateStudent(list, "GuakHyeonSu", "7676767676");
	CreateStudent(list, "JeongHyeonSu", "6464646464");
	CreateStudent(list, "KownMinSu", "5353535353");
	CreateStudent(list, "KimMinSu", "8484848484");
	CreateStudent(list, "ParkMinSu", "8585858585");
	CreateStudent(list, "GuakMinSu", "1212121212");
	
	printf("\n0 : Quit Program\n\n-----List-----\n\n");
	Nprint(list); //����Ʈ ����� ����Ʈ �Ѵ�. 
	printf("\n");
	while(strcmp(search, "0") != 0){
		printf("Search : ");
		scanf("%s", search);
		if(strcmp(search, "0") != 0) Nsearch(list,search); //���� search������ 0�ϰ�� while���� ����������, �ƴϸ� Nsearch�Լ��� �����Ѵ�. 
		
	}
	 //�����ϰ� �Ǹ� �����Ҵ��� �������־�� �ϹǷ�, list�� �����Ҵ��� �������ְ�(destroylist), search������ list���� �����ش�. 
	Destroylist(list);
	free(search);
	free(list);
	printf("\nMemory deallocated, Exit Program\n\n");
	return 0;
}
