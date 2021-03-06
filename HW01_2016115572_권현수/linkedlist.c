//Linkedlist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
//student 구조체 생성 
typedef struct _Student{
	char name[20];
	char number[10];
}Student;
 
//student라는 구조체를 갖는 노드 생성 
typedef struct _Node{
	Student *data;
	struct _Node *next;
}Node;

//노드를 갖는 리스트 생성 - 구성요소 cur, head, tail 
typedef struct _list{
	Node *cur, *head, *tail;
}List;

//main에서 만든 list와, main에서 정해준 name 과 number를 이용하여 list안에 student를 만들어서
//name과 number를 넣는다. 
void CreateStudent(List* list, char* name, char* number){
	
	//새로만든 노드와 student를 동적할당해준다. 
	Node *newNode = (Node*)malloc(sizeof(Node));
	if(newNode == NULL) return;
	
	Student *newStudent = (Student*)malloc(sizeof(Student));
	if(newStudent == NULL) return;
	
	strcpy(newStudent->name, name);
	strcpy(newStudent->number, number);
	
	//새로운 노드에 값들을 집어넣는다. 
	newNode->data = newStudent;
	newNode->next = NULL;
	
	//list에 아무것도 없을 경우, list의 head와 tail을 새로운 노드로 넣는다. 
	if(list->head == NULL && list->tail == NULL) list->head = list->tail = newNode;
	else{//그렇지 않을 경우, 꼬리쪽에 새로운 노드를 넣는다. 
		list->tail->next = newNode;
		list->tail = newNode;
	}
}

//노드를 파괴하는 함수  
void Destroylist(List* list){
	//지울 노드와 그 다음 찾아가는 노드 
	Node *delNode = list->head;
	Node *NextNode = delNode->next;
	
	
	while(NextNode!=NULL){ //nextnode가 null일 경우 스탑 
		
		delNode->data = NULL;
		free(delNode->data); //node안에있는 data 즉, student를 할당해제한다. 
		
		free(delNode); //노드 자체를 할당해제한다. 
		delNode = NextNode;
		NextNode = NextNode->next; //nextnode를 다음노드로 이동하여 모든 노드를 찾게 한다. 
	}
	free(delNode);
	free(NextNode); 
}

//list안에있는 모든 노드를 검색하여 노드 안에있는 student(data)의 name과 number를 출력한다. 
void Nprint(List* list){
	Node *Ntemp = list->head;
	while(Ntemp != NULL){
		printf("%s, %s\n",Ntemp->data->name, Ntemp->data->number);
		Ntemp = Ntemp->next;
	}
}

//list안에있는 노드의 student의 name과 number가 search값과 같은것이 있는지 확인하게 한다. 
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
	char *search = (char*)malloc(sizeof(char)*100); //찾고자하는 값을 넣을 변수. 동적할당함. 
	
	//list를 선언해주고, cur, head, tail값을 null로 초기화 해준다. 
	List *list = (List*)malloc(sizeof(List));
	list->cur = NULL;
	list->head = NULL;
	list->tail = NULL;
	
	printf("linkedlist\n\n");
	
	//초기화된 list에 각각 이름과 학번을 넣는다. 
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
	Nprint(list); //리스트 목록을 프린트 한다. 
	printf("\n");
	while(strcmp(search, "0") != 0){
		printf("Search : ");
		scanf("%s", search);
		if(strcmp(search, "0") != 0) Nsearch(list,search); //만약 search변수가 0일경우 while문을 빠져나오고, 아니면 Nsearch함수를 실행한다. 
		
	}
	 //종료하게 되면 동적할당을 해제해주어야 하므로, list의 동적할당을 해제해주고(destroylist), search변수와 list또한 없애준다. 
	Destroylist(list);
	free(search);
	free(list);
	printf("\nMemory deallocated, Exit Program\n\n");
	return 0;
}
