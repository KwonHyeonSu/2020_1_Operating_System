#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

//polling
//switch -> GPIO21
//led -> GPIO25

void main(){
	char buf[BUFSIZ];
	char i = 0;
	int fd = -1;
	int fd2 = -1;
	char option[3];

	fd = open("/dev/ledtest", O_RDWR); // dev경로의 ledtest 장치파일을 불러옵니다. 
	printf("fd : %d\n", fd);
	
	printf("push swtich -> led on\n");
	
	printf("ctrl+Z to quit\n");
	while(1){ //반복문을 돌려서 스위치의 현재 상태를 계속해서 받아오고, 이를 이용해 led를 켜고 끕니다. 
		fd2 = popen("gpio -g read 21", "r");
		fgets(option, 2, fd2);
		if(strcmp(option,"0")==0){
			strcpy(buf, "1");
			write(fd, buf, strlen(buf), NULL);
		}else if(strcmp(option,"1")==0){
			strcpy(buf, "0");
			write(fd, buf, strlen(buf), NULL);
		}
		usleep(50000); //cpu의 부하를 막기위해 조금의 딜레이를 줍니다. 
		close(fd2);
	}
	printf("program exit\n");
	return;
}

