#include  <stdio.h>
#include  <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define   ONE_MEG              1048576
//  Prototypes
void     RecursiveRoutine( int );
char     *commas(unsigned long amount);
//  This is a global so the subroutine can see it.
unsigned long    FirstStackLocation;
unsigned long    temp;
int msgid;

struct msgbuf{ //메시지큐 구조체
	long mtype;
	unsigned long data;
};
struct msgbuf mbuf;
int    main()
{
    int    TopOfStack;
    int    Counter = 0;
	

	if(-1 == (msgid = msgget((key_t)16, IPC_CREAT|0666))){ //스택이 사용할 메시지큐를 생성합니다.
		printf("msgget failed\n");
		exit(1);
	}
    FirstStackLocation = (unsigned long)(&TopOfStack);
    printf("First location on stack: %s\n", 
            commas( (unsigned long)FirstStackLocation  ) );
    RecursiveRoutine( 0 );
    
    printf("Stacking process exit!\n");
    return 0;
    
}

///////////////////////////////////////////////////////////////////////////
//  This routine is called recursively.  It does no real work but adds
//  a variable to the stack which uses up space onthe stack.
//  It's expected that the program will crash when memory is exhausted.
///////////////////////////////////////////////////////////////////////////
#define    STACK_ALLOC    ONE_MEG/20
void  RecursiveRoutine( int RecursiveDepth )
{
    char    Temp[ STACK_ALLOC ];
    char    StringTop[32];
    char    StringBottom[32];
    
    strcpy( StringTop,  commas( (unsigned long)(FirstStackLocation) ) );
    strcpy( StringBottom, commas( (unsigned long)&(Temp[STACK_ALLOC]) ) );
    printf("Iteration = %3d:  Stack Top/Bottom/Bytes: %s  %s  %ld\n", 
             RecursiveDepth, StringTop, StringBottom,
             FirstStackLocation - (unsigned long)&(Temp[STACK_ALLOC])  );
             
    mbuf.mtype = 1;     //메시지큐 구조체 타입은 1로 정해줍니다.
    mbuf.data = (unsigned long)&Temp[STACK_ALLOC]; //메시지큐 데이터에 값을 넣어줍니다.
    //메시지큐에 메시지를 보냅니다.
    if(-1 == msgsnd(msgid, (void*)&mbuf, sizeof(mbuf)-sizeof(long), 0)){
		printf("msgsnd failed\n");
		exit(1);
	}
             
    RecursiveDepth++;
    RecursiveRoutine( RecursiveDepth );
}


///////////////////////////////////////////////////////////////////////////
//  The following routines are for formatting only and aren't needed for
//  an understanding of the memory manipulations done above.
///////////////////////////////////////////////////////////////////////////
#define BASE 16
#define GROUP 4
// This routine converts an unsigned integer into a string with commas.
// You may need to adjust the base and the number of digits between
// commas as given by BASE and GROUP.
// Need space to hold the digits of an unsigned int,
// intervening commas and a null byte. It depends on
// BASE and GROUP above (but logarithmically, not
// as a constant. so we must define it manually here)
///////////////////////////////////////////////////////////////////////////
#define MAXTEXT 25 

int prepend(char *, unsigned, char *);
int preprintf(char *, unsigned, char *, ...);

char *commas(unsigned long amount)
    {
    short i;
    short offset = MAXTEXT-1;   /* where the string "starts"  */
    short place;                /* the power of BASE for      */
                                /* current digit              */
    static char text[MAXTEXT];

    for ( i = 0; i < MAXTEXT; i++ )
        text[i] = '\0';
					     
   /* Push digits right-to-left with commas */
   for (place = 0; amount > 0; ++place)
      {
      if (place % GROUP == 0 && place > 0)
         offset = prepend(text,offset,",");
      offset = preprintf(text,offset,"%X",amount % BASE);
      amount /= BASE;
    }
    return (offset >= 0) ? text + offset : NULL;
}
    /* preprint.c: Functions to prepend strings */
    


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
    
int prepend(char *buf, unsigned offset, char *new_str)
    {
    int new_len = strlen(new_str);
    int new_start = offset - new_len;
    /* Push a string onto the front of another */
    if (new_start >= 0)
        memcpy(buf+new_start,new_str,new_len);
    
    /* Return new start position (negative if underflowed) */
    return new_start;
}
    
int preprintf(char *buf, unsigned offset, char *format, ...)
    {
    int pos = offset;
    char *temp = malloc(BUFSIZ);
    
    /* Format, then push */
    if (temp)
        {
        va_list args;
        
        va_start(args,format);
        vsprintf(temp,format,args);
        pos = prepend(buf,offset,temp);
        va_end(args);
        free(temp);
    }
    return pos;
}


