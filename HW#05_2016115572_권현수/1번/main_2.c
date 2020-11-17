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

	fd = open("/dev/ledtest", O_RDWR); // dev����� ledtest ��ġ������ �ҷ��ɴϴ�. 
	printf("fd : %d\n", fd);
	
	printf("push swtich -> led on\n");
	
	printf("ctrl+Z to quit\n");
	while(1){ //�ݺ����� ������ ����ġ�� ���� ���¸� ����ؼ� �޾ƿ���, �̸� �̿��� led�� �Ѱ� ���ϴ�. 
		fd2 = popen("gpio -g read 21", "r");
		fgets(option, 2, fd2);
		if(strcmp(option,"0")==0){
			strcpy(buf, "1");
			write(fd, buf, strlen(buf), NULL);
		}else if(strcmp(option,"1")==0){
			strcpy(buf, "0");
			write(fd, buf, strlen(buf), NULL);
		}
		usleep(50000); //cpu�� ���ϸ� �������� ������ �����̸� �ݴϴ�. 
		close(fd2);
	}
	printf("program exit\n");
	return;
}

