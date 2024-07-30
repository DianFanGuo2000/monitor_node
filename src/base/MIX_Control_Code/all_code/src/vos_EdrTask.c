/******************************************************************************
 *File Name    :   vos_EdrTask.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2013/04/01
 *Description  :    generate task create function.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "vos_EdrTask.h"
#include "vos_EdrMessage.h"


#include "stdio.h"
#ifdef MNG_LINUX_VER
#include <limits.h>
#include <sys/prctl.h>
#include <pthread.h>
#endif
#include <errno.h>

#ifdef MNG_VXWORKS_VER
#include "taskLib.h"
#endif

/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/

/******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/



/******************************************************************************
 *    Local Struct  Define Section
 ******************************************************************************/

/******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/



/******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/

/******************************************************************************
 *    Function Define Section
 ******************************************************************************/
/******************************************************************************
 * Name:INT32 AdjustTaskPriority(INT32 Priority_I32)
 * Function: adjust the task priority value.
 * Input:    Priority_I32,the task priority;
 * return : the  have adjusted priority.
 * Create by moyun  2014-7-23
 *******************************************************************************/
static INT32 AdjustTaskPriority(INT32 Priority_I32)
{
    INT32 RetVal_I32 = 0; /*define return value variable*/
#ifdef MNG_VXWORKS_VER
    /*the new priority*/
    RetVal_I32=VOS_TASK_BASE_PRIORITY +Priority_I32;
#endif
#ifdef MNG_LINUX_VER
    /*the new priority*/
    RetVal_I32=Priority_I32;
#endif
    /*return the priority*/
    return RetVal_I32;
}

/******************************************************************************
 * Name:  STATUS vos_EdrTaskSpawn(const char *TaskName_pc8,INT32 Priority_I32,\
                               INT32 Policy_I32, INT32 StackSize_I32, FUNCPTR exec_fun,INT32 Par_I32)
 * Function: edr task create function ,this function execute the same function of the taskspawn.
 * Input:  TaskName_pc8,task name.
 *            Priority_I32,task priority.
               Policy_I32, task policy.
               StackSize_I32,stack size.
 *            ExecFunction_pFun,execute function ptr.
 *            ParVal_I32,parameter val.
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
INT32 vos_EdrTaskSpawn(const char *TaskName_pc8,INT32 Priority_I32,INT32 Policy_I32, INT32 StackSize_I32, FUNCPTR exec_fun,INT32 Par_I32)
{
    INT32 AdjustPriority_I32=0;
#ifdef MNG_VXWORKS_VER
    INT32 RetVal_I32=ERROR;
    AdjustPriority_I32=AdjustTaskPriority(Priority_I32);
    RetVal_I32=taskSpawn(TaskName_pc8,AdjustPriority_I32,Policy_I32,StackSize_I32,exec_fun,\
                         Par_I32,0,0,0,0,0,0,0,0,0);
    return RetVal_I32;
#endif
#ifdef MNG_LINUX_VER
    pthread_t Thread_ST;  /*Thread_ST variable*/
    pthread_attr_t ThreadAttrib_ST;   /*the therad attribute variable*/
    struct sched_param SchedParam_ST;  /* scheduling priority */
    INT32 RetCode_I32=ERROR;      /*the return value*/
    printf("vos_EdrTaskSpawn 0\r\n");
    RetCode_I32 = pthread_attr_init(&ThreadAttrib_ST);
    if (RetCode_I32 != 0)
    {
        printf("trdp_vosThreadSpawn pthread_attr_init failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 1\r\n");
    /* Set stack size */
    if (StackSize_I32 > PTHREAD_STACK_MIN)
    {
        RetCode_I32 = pthread_attr_setstacksize(&ThreadAttrib_ST, StackSize_I32);
    }
    else
    {
        RetCode_I32 = pthread_attr_setstacksize(&ThreadAttrib_ST, PTHREAD_STACK_MIN);
    }
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_attr_setstacksize failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 2\r\n");
    /* Detached thread */
    RetCode_I32 = pthread_attr_setdetachstate(&ThreadAttrib_ST, PTHREAD_CREATE_DETACHED);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_attr_setdetachstate failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 3\r\n");
    /* Set the policy of the thread */
    // RetCode = pthread_attr_setschedpolicy(&ThreadAttrib, SCHED_FIFO);
    /*timer slice*/
    RetCode_I32 = pthread_attr_setschedpolicy(&ThreadAttrib_ST, SCHED_RR);
    //  RetCode = pthread_attr_setschedpolicy(&ThreadAttrib, SCHED_OTHER);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_attr_setschedpolicy failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }

printf("vos_EdrTaskSpawn 4\r\n");
    /* Set the scheduling priority of the thread */
    AdjustPriority_I32=AdjustTaskPriority(Priority_I32);
    SchedParam_ST.sched_priority =  AdjustPriority_I32;
    RetCode_I32= pthread_attr_setschedparam(&ThreadAttrib_ST, &SchedParam_ST);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn  pthread_attr_setschedparam failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 5\r\n");
    /*set the thread compete scope PTHREAD_SCOPE_SYSTEM,PTHREAD_SCOPE_PROCESS
       add by moyun 2016-07-20*/
    RetCode_I32 = pthread_attr_setscope(&ThreadAttrib_ST, PTHREAD_SCOPE_SYSTEM);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_attr_setscope failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 6\r\n");
    /* Set inheritsched attribute of the thread */
    RetCode_I32 = pthread_attr_setinheritsched(&ThreadAttrib_ST, PTHREAD_EXPLICIT_SCHED);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_attr_setinheritsched failed Linux return=%d\n", RetCode_I32 );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 7\r\n");
    /* Create the thread */
    //RetCode_I32 = pthread_create(&Thread_ST, &ThreadAttrib_ST,&exec_fun,(void*)Par_I32);
    RetCode_I32 = pthread_create(&Thread_ST, NULL,exec_fun,NULL);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_create failed Linux return=%d error str %s \n", RetCode_I32,strerror(errno) );
        return(ERROR);
    }
printf("vos_EdrTaskSpawn 8\r\n");
    /* Destroy thread attributes */
    RetCode_I32 = pthread_attr_destroy(&ThreadAttrib_ST);
    if (RetCode_I32 != 0)
    {
        printf("linux_task_spawn pthread_attr_destroy Linux failed return=%d\n", RetCode_I32 );
        return(ERROR);
    }
    return Thread_ST;  /*lint !e429 */
#endif
}


/******************************************************************************
 * Name:void SetThreadName(char *TaskName_pC8)
 * Function: set the thread name.
 * Input:    TaskName_pC8,the task name;
 * return : non
 * Create by moyun  2014-7-23
 *******************************************************************************/
void SetThreadName(char *TaskName_pC8)
{
    /*set the thread name*/
#ifdef MNG_LINUX_VER
    prctl(PR_SET_NAME,TaskName_pC8);
#endif
}

/******************************************************************************
 * Name:  BOOL vos_IsEdrTaskExist( INT32 ThreadId_I32)
 * Function:check the thread exist or not.
 * Input:
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/

BOOL vos_IsEdrTaskExist( INT32 ThreadId_I32)
{
	
#ifdef MNG_LINUX_VER
    int pthead_kill_err;

    BOOL RetVal_B = FALSE;

    pthread_t Thread_ST;

    if(ThreadId_I32 == -1)
    {
        return FALSE;
    }

    Thread_ST = (pthread_t)ThreadId_I32;

    pthead_kill_err = pthread_kill(Thread_ST, 0);

	//printf("pthead_kill_err :%d\n",pthead_kill_err);

    if(pthead_kill_err == ESRCH)
    {
        RetVal_B = FALSE;
    }
    else if(pthead_kill_err == EINVAL)
    {
        printf("task sig invalid!\n");
        RetVal_B = FALSE;
    }
    else
    {
        RetVal_B = TRUE;
    }

    return RetVal_B;
#endif

}






