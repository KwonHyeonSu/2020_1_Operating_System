#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>

#define   ONE_MEG   1048576
///////////////////////////////////////////////////////////////////////////
//  Prototypes  &  Globals
///////////////////////////////////////////////////////////////////////////
long       *MemoryPtr;

struct msgbuf{ //메시지큐 구조체
	long mtype;
	unsigned long data;
};

///////////////////////////////////////////////////////////////////////////
//  The main code is here
///////////////////////////////////////////////////////////////////////////
void main(int argc, char *argv[]){

    long         NumberOfMegaBytes, NumberOfAllocations = 0;
    long         Temp;
    long         *TempPointer;
    char         MemoryFunction[32];
    int          Function = 0;
    
    int msgid; //메시지큐아이디 변수
    struct msgbuf mbuf; //메시지큐 구조체 변수
    
    if (argc < 3 ) {
        printf( "Usage: %s <MemorySizeInMegabyte> <Read|Write|Nothing>\n", argv[0]);
        exit(1);
    }
    
    msgid = msgget((key_t)32, IPC_CREAT|0666); //힙이 사용할 메시지큐를 생성합니다.
    if(msgid<0){
    	perror("msgid failed!\n");
    	exit(1);
    }
    
    //메시지큐 구조체에 들어갈 내용을 초기화합니다. type은 1로 합니다.
    mbuf.mtype = 1;
    mbuf.data = 0;
    
    //  Get the arguments and validate them
    NumberOfMegaBytes = atoi( argv[1] );
    strcpy( MemoryFunction, argv[2] );
    if ( strncmp ( MemoryFunction, "Nothing", 7 ) == 0 ) Function = 1;
    if ( strncmp ( MemoryFunction, "Read", 4 ) == 0    ) Function = 2;
    if ( strncmp ( MemoryFunction, "Write", 5 ) == 0   ) Function = 3;
    if ( Function == 0 )  {
        printf( "Unable to recognize the Read|Write|Nothing portion of the command\n");
	return;
    }
    //  Now loop through each of the megabytes and perhaps touch each of the 1024 pages
    //  in that megabyte.
    while ( NumberOfAllocations < NumberOfMegaBytes ) {
        MemoryPtr = (long *)malloc(ONE_MEG);
        if ( MemoryPtr == 0 ) {
            printf( "The program is ending because we could allocate no more memory.\n");
	    printf( "Total megabytes allocated = %ld\n", NumberOfAllocations );
            exit(0);
        }
	NumberOfAllocations++;
	if ( ( NumberOfAllocations % 100 ) == 0 )    // Print out status every so often
		printf( "We have allocated %ld Megabytes\n", NumberOfAllocations );
		
	
	if((unsigned long)MemoryPtr >= mbuf.data){ //만약 MemoryPtr이 저장된 data보다 클 경우
		if(-1 == msgctl(msgid, IPC_RMID, NULL)){ //메시지큐를 삭제합니다.
			perror("msgctl failed!\n");
			exit(1);
		}
		mbuf.data = 0; //데이터를 초기화해주고,
		//삭제되었음을 알려줍니다. (디버깅용 - 보기편하기위해 일부러 적어놓았습니다.)
		printf("removed!\n"); 
		//메시지큐가 지워졌으므로 메시지큐를 새로 생성합니다.
		msgid = msgget((key_t)32, IPC_CREAT|0666); 
		    if(msgid<0){
		    	perror("msgid failed!\n");
		    	exit(1);
		    }
		//데이터를 메모리 포인터 값으로 넣어줍니다.
		mbuf.data = (unsigned long)MemoryPtr;
		
		//메시지큐에 해당 값을 넣어줍니다.
		if(-1 == msgsnd(msgid, (void*)&mbuf, sizeof(mbuf)-sizeof(long), 0)){
		    	perror("msgsnd failed!\n");
		    	exit(1);
   		}
		
	}
	

	TempPointer = MemoryPtr;
	if ( Function == 2 )   {                     // Read from each page in the megabyte
	    while ( (unsigned long)TempPointer < (unsigned long)((long)MemoryPtr + ONE_MEG) )  {
                Temp                   = TempPointer[0];
                TempPointer = (long *)((unsigned long)TempPointer +4096);
                
            }          // End of while
        }              // End of if Function is Read

	if ( Function == 3 )    {                    // Write to each page in the megabyte
	    while ( (unsigned long)TempPointer < (unsigned long)((long)MemoryPtr + ONE_MEG) )  {
                TempPointer[0] = Temp;
                TempPointer = (long *)((unsigned long)TempPointer +4096);
            }          // End of while
        }              // End of if Function is Write
    }                  // End of while memory still can be allocated
    
    printf("Heap process exit\n");
    return;

}                             // End of main


