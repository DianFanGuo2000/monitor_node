
/*

#include "stdio.h"
#include <pthread.h>



typedef int             INT32;
#define STATUS  int



//extern INT32  udpCanStart(void);
extern STATUS canAndCanFdTest(void);
extern STATUS vos_EdrTaskDelay(INT32);



void *ThreadTest(void *arg)
{


printf("ThreadTest ok \r\n");



}







INT32  main(INT32 Argc_I32,char **argv_pST)
{
pthread_t tid;

	printf("x86 start \r\n");

    //udpCanStart();
        
   // canAndCanFdTest();

pthread_create(&tid,NULL,ThreadTest,NULL);



        
	while(1)
	{
		vos_EdrTaskDelay(5000);
	}
	
        return 1;
}

*/

