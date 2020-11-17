#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

void main(){
	char buf[BUFSIZ];
	char i = 0;
	char temp[BUFSIZ];
	int fd = -1;
	int j;
	int flag = 0;
	int option;
	char* input = (char*)malloc(sizeof(char)*100);
	if(input == NULL){
		perror("malloc error : ");
		exit(1);
	}
	
	while(1){ //반복문을 돌립니다. 
		printf("Input (A/B) Quit to (C): ");
		scanf("%s", input); //인풋 값을 받습니다. 
		if(strcmp(input, "A") == 0){ //인풋 값이 A라면 led를 3번 깜박입니다. 
			printf("A inputed!\n");
			fd = open("/dev/ledtest", O_RDWR);
			printf("fd : %d\n", fd);
			for(j=0;j<3;j++){
				strcpy(buf, "1");
				write(fd, buf, strlen(buf), NULL);
				usleep(500000); //0.5초 쉽니다. 
				strcpy(buf, "0");
				write(fd, buf, strlen(buf), NULL);
				usleep(500000);
			}
			close(fd);
			
		}else if(strcmp(input, "B") == 0){ //인풋값이 B일 때  
			printf("B inputed!\n");
			fd = open("/dev/ledtest", O_RDWR);
			printf("fd : %d\n", fd);

			while(1){
				printf("0 : led off, 1 : led on, 5 : Quit\n");
				read(fd, temp, BUFSIZ, NULL); // read
				printf("temp : %s\n", temp);

				printf("option : ");
				
				scanf("%d", &option); //1을 눌렀을 때 LED가 켜지고, 0일때 꺼집니다. 이 값을 OPTION 변수에 저장합니다.  
				if(option == 1){
					strcpy(buf, "1");
					write(fd, buf, strlen(buf), NULL);
				}else if(option == 0){
					strcpy(buf, "0");
					write(fd, buf, strlen(buf), NULL);
				}else if(option == 5){
					flag = 1;
					break;
				}
				else{
					printf("must be 0 or 1");
				}
			}
			close(fd);
			
			if(flag == 1) break; //반복문을 빠져나가기 위함입니다. 
		}else if(strcmp(input, "C") == 0){
			break;
		}else{
			printf("Input must be A or B! Type again!\n");
		}
	}
	
	printf("program exit\n");
	return;
}

