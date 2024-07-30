/******************************************************************************

 *File Name    : vos.c

 *Copyright    : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.

 *Create Date  : 2012/3/14

 *Description  : virtual OS.

 *REV1.0.0     : lisy   2012/3/14  File Create
 *REV1.1.0     : lisy   2013/5/28 add Linux version


******************************************************************************/

/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "mng_Interface.h"
#include "vos.h"
#include "vos_Print.h"
#include "vos_com.h"
#include "vos_Define.h"
/******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/
VOS_STRUCT g_vos_ST;  /*the vos struct*/
/******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/

/******************************************************************************
 *    Local Structure Define Section
 ******************************************************************************/

typedef struct threadArgs
{
    VOS_THREAD_ROUTINE theFunction;
    void *pArguments;
} VOS_THREAD_ARGS;

typedef struct cyclicThreadArgs
{
    void (*pFunc)(void);
    int interval;
} CYCLIC_THREAD_ARGS;

/******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/
UINT32 vos_GetMilliSecTimer(void);
/******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/

#if defined(VXWORKS)
static UINT32 timeStampFreq;
#endif

static int threadSystemShutdown = FALSE;

/* added for CR-685 (what does this mean?) */
static int threadsRunning = 0;

#if defined(LINUX)
/* File path to writable file that is used to generate a unique number.
   For identification of message queues and shared memory.
   If the file do not exist it will be created.
   The file name and path may be set in the xml configuration file */
static char linuxFile[LINUX_FILE_SIZE] = "/tmp/msgq";
#endif

/******************************************************************************
 *    Function Define Section
 ******************************************************************************/

/******************************************************************************

 *Name        : cyclicThread

 *Function    :  cyclic thread, run when time is up

 *para        :

 *return      :

 *Created by Lisy 2012/8/6
 ******************************************************************************/
static void cyclicThread
(
    void *pArg
)
{
    CYCLIC_THREAD_ARGS *pArgs = NULL;
    UINT32 nextTime;
    UINT32 currentTime;
    UINT32 delayTime;
    UINT32 cycleTime;
    /* verify that pointer is valid */
    if(NULL == pArg)
    {
        return;
    }
    pArgs = (CYCLIC_THREAD_ARGS *)pArg;
    cycleTime = pArgs->interval;
    nextTime = vos_GetMilliSecTimer();
    while(0==threadSystemShutdown)
    {
        /* call the thread function */
        pArgs->pFunc();
        /* calculate time for next cycle */
        nextTime = nextTime + cycleTime;
        currentTime = vos_GetMilliSecTimer();
        if(currentTime < nextTime)
        {
            delayTime = nextTime - currentTime;
            if(delayTime > cycleTime)
            {
                /* the clock has been changed */
                delayTime = cycleTime;
                nextTime = currentTime + cycleTime;
            }
        }
        else
        {
            /* the clock has been changed or the thread function takes longer time
             * than the interval time. Use the interval time as delay value. */
            delayTime = cycleTime;
            nextTime = currentTime + cycleTime;
        }
        vos_TaskDelay(delayTime);
    }
    vos_Free((BYTE *)pArgs);
    VOS_PRINTF("cyclic thread exits\n");
    threadsRunning--;
}



/******************************************************************************

 *Name        : routineStarter

 *Function    :  start routine to vos_ThreadSpawn

 *para        :

 *return      :

 *Created by Lisy 2012/8/3
 ******************************************************************************/
static void routineStarter
(
    void *pArguments                /* pointer to thread arguments */
)
{
    VOS_THREAD_ARGS *pVTA = (VOS_THREAD_ARGS *)pArguments;
    /* call task */
    pVTA->theFunction(pVTA->pArguments);
    vos_Free((BYTE *)pArguments);
}



/******************************************************************************

 *Name        : vos_SystemStartup

 *Function    : VOS system start up

 *para        :

 *return      : 0 if OK,!=0 if error

 *Created by Lisy 2012/7/12

 ******************************************************************************/
#if defined(LINUX)
int vos_SystemStartup
(
    char *path
)
#else
int vos_SystemStartup(void)
#endif
{
#if defined(LINUX)
    FILE *stream;
    if((NULL != path) && (strlen(path) > 0))
    {
        strncpy(linuxFile, path, sizeof(linuxFile) - 1);
    }
    /* create msgq file */
    if(NULL == (stream = fopen(linuxFile, "a")))
    {
        VOS_PRINT1(VOS_ERR, "vos_SystemStartup:could not create file %s\n", linuxFile);
        return (int)VOS_ERROR;
    }
    else
    {
        fclose(stream);
    }
#elif defined(VXWORKS)
    int prevIOCTL;
    int newIOCTL;
    /* return the current device options  */
    prevIOCTL=ioctl((int)stdin, FIOGETOPTIONS, 0);
    /*          OPT_ECHO        0x01         echo input
                OPT_CRMOD       0x02         lf -> crlf
                OPT_TANDEM      0x04         ^S/^Q flow control protocol
                OPT_7_BIT       0x08         strip parity bit from 8 bit input
                OPT_MON_TRAP    0x10         enable trap to monitor
                OPT_ABORT       0x20         enable shell restart
                OPT_LINE        0x40         enable basic line protocol */
    newIOCTL=prevIOCTL & ~OPT_LINE;
    /*  prevIOCTL, newIOCTL is not used actually */
    /* get the timestamp timer clock frequency, in ticks per second
     * 33000000, 33Hz */
    timeStampFreq=sysTimestampFreq();
    if(0 == timeStampFreq)
    {
        VOS_PRINT0(VOS_ERR, "vos_SystemStartup: timeStampFreq is 0\n");
        return((int)RET_ERR);
    }
#endif
    return (int)RET_OK;
    /* this routine just get the timestamp timer clock frequency by the return value
     * of sysTimestampFreq(), and to check whether the system has started up */
}



/******************************************************************************

 *Name        : vos_SystemShutdown

 *Function    : shutdown system

 *para        :

 *return      :

 *Created by Lisy 2012/10/23

 ******************************************************************************/
void  vos_SystemShutdown(void)
{
#if defined(WIN32)
#elif defined(LINUX)
    /* shutdown curses */
#elif defined(VXWORKS)
    /* noting to do */
#elif defined(__INTEGRITY)
    /* nothing to do */
#endif
}



/******************************************************************************

 *Name        : vos_TaskDelay

 *Function    : Put the calling thread to sleep for specified no of millisecs

 *para        :

 *return      :

 *Created by Lisy 2012/7/18

 ******************************************************************************/
void vos_TaskDelay
(
    int delayInMs         /* delay in milliseconds */
)
{
#if defined(LINUX)
#if 1
    int ret;
    struct timespec delayTime;
    struct timespec remainTime;
    if (delayInMs >= 1000 )
    {
        /*the second should be div 1000*/
        delayTime.tv_sec = delayInMs / 1000;
        delayTime.tv_nsec = (delayInMs % 1000) * 1000000;
    }
    else
    {
        delayTime.tv_sec = 0;
        delayTime.tv_nsec = delayInMs * 1000000;
    }
    ret = nanosleep(&delayTime, &remainTime);
    while ((ret != 0) && (errno == EINTR))//(ret == -1)
    {
        /* The sleep was interrupted,
          Wait the remaining time */
        ret = nanosleep(&remainTime, &remainTime);
    }
#else
    Sleep(delayInMs);
#endif
#elif defined(VXWORKS)
    UINT32 clockRate;
    UINT32 noTicks;
    /* get the ticks per second */
    clockRate = sysClkRateGet();
    /* computer how many ticks in dalayInMs milliseconds */
    noTicks = (clockRate * delayInMs + 999) / 1000;
    taskDelay(noTicks);
#endif
}



/******************************************************************************

 *Name        : vos_RegisterCyclicThread

 *Function    :  spawn a thread

 *para        :

 *return      :   Thread ID

 *Created by Lisy 2012/8/3
 ******************************************************************************/
void vos_RegisterCyclicThread
(
    void (*pFunc)(void),     /* pointer to thread function */
    char *pName,          /* thread name */
    int interval,         /* thread cycle time */
    int policy,           /* thread policy */
    int priority,         /* priority */
    int stackSize         /* stack size */
)
{
    CYCLIC_THREAD_ARGS *pArg = NULL;
    threadsRunning++;
    pArg = (CYCLIC_THREAD_ARGS *)vos_MemAlloc(sizeof(CYCLIC_THREAD_ARGS));
    if(NULL != pArg)
    {
        pArg->pFunc = pFunc;
        pArg->interval = interval;
        (void)vos_ThreadSpawn(pName, policy, priority, stackSize, &cyclicThread, pArg);
    }
}



/******************************************************************************

 *Name        : vos_ThreadRun

 *Function    :  run the thread

 *para        :

 *return      :

 *Created by Lisy 2013/6/14
 ******************************************************************************/
void vos_ThreadRun(void)
{
    threadSystemShutdown = FALSE;
}



/******************************************************************************

 *Name        : vos_ThreadTerminate

 *Function    :  terminate the thread

 *para        :

 *return      :

 *Created by Lisy 2012/8/7
 ******************************************************************************/
void vos_ThreadTerminate(void)
{
    threadSystemShutdown = TRUE;
}



/******************************************************************************

 *Name        : vos_ThreadDone

 *Function    :  waits for termination of threads

 *para        :

 *return      :

 *Created by Lisy 2012/8/7
 ******************************************************************************/
void vos_ThreadDone(void)
{
    VOS_PRINTF("waiting for thread termination\n");
    while(threadsRunning)
    {
        vos_TaskDelay(500);
    }
    VOS_PRINTF("cyclic Thread Done\n");
}



/******************************************************************************

 *Name        : vos_ThreadSpawn

 *Function    :  spawn a thread

 *para        :

 *return      :   Thread ID

 *Created by Lisy 2012/8/3
 ******************************************************************************/
VOS_THREAD_ID vos_ThreadSpawn
(
    char *pName,                            /* thread name */
    int policy,                            /* thread policy */
    int priority,                          /* priority */
    int stackSize,                         /* stack size */
    VOS_THREAD_ROUTINE theFunction,       /* pointer to thread main function */
    void *pArguments                       /* pointer to thread arguments */
)
{
    VOS_THREAD_ARGS *pVTrArgs = NULL;
#if defined(LINUX)
    pthread_t hThread;
    pthread_attr_t ThreadAttrib;
    struct sched_param SchedParam;  /* scheduling priority */
    int RetCode;
    VOS_UNUSED(pName);
#elif defined(VXWORKS)
    VOS_THREAD_ID threadID;
    char errBuf[NAME_MAX];
    VOS_UNUSED(policy);
#endif
    pVTrArgs = (VOS_THREAD_ARGS *)vos_MemAlloc(sizeof(VOS_THREAD_ARGS));
    if(NULL != pVTrArgs)
    {
        pVTrArgs->theFunction = theFunction;
        pVTrArgs->pArguments = pArguments;
    }
    else
    {
        VOS_PRINT1(VOS_ERR, "vos_Malloc ERROR, %d bytes required", sizeof(VOS_THREAD_ARGS));
    }
#if defined(LINUX)
    /* Initialize thread attributes to default values */
    RetCode = pthread_attr_init(&ThreadAttrib);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_init failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Set stack size */
    if (stackSize > PTHREAD_STACK_MIN)
    {
        RetCode = pthread_attr_setstacksize(&ThreadAttrib, stackSize);
    }
    else
    {
        RetCode = pthread_attr_setstacksize(&ThreadAttrib, PTHREAD_STACK_MIN);
    }
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_setstacksize failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Detached thread */
    RetCode = pthread_attr_setdetachstate(&ThreadAttrib, PTHREAD_CREATE_DETACHED);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_setdetachstate failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Set the policy of the thread */
    RetCode = pthread_attr_setschedpolicy(&ThreadAttrib, policy);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_setschedpolicy failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Set the scheduling priority of the thread */
    SchedParam.sched_priority = priority;
    RetCode = pthread_attr_setschedparam(&ThreadAttrib, &SchedParam);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_setschedparam failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Set inheritsched attribute of the thread */
    RetCode = pthread_attr_setinheritsched(&ThreadAttrib, PTHREAD_EXPLICIT_SCHED);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_setinheritsched failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Create the thread */
    //RetCode = pthread_create(&hThread, &ThreadAttrib, (void *)routineStarter, (void *)pVTrArgs);
    RetCode = pthread_create(&hThread, NULL, (void *)routineStarter, (void *)pVTrArgs);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_create failed Linux return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    /* Destroy thread attributes */
    RetCode = pthread_attr_destroy(&ThreadAttrib);
    if (RetCode != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_attr_destroy Linux failed return=%d\n", RetCode );
        vos_Free((BYTE *) pVTrArgs);
        return(0);
    }
    return hThread;  /*lint !e429 */
    /* lint warning "Custodial pointer 'pVTrArgs' has not been freed or returned" */
    /* becaused pVTrArgs is not returned. The memory is returned in RoutineStarter instead */
#elif defined(VXWORKS)
    threadID = taskSpawn(pName, priority, VOS_VX_FP_TASK, stackSize, (FUNCPTR)&routineStarter,
                         (int)pVTrArgs, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if(ERROR == threadID)
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "taskSpawn failed. VxWorks errno=%#x %s\n", errno, errBuf);
        vos_Free((BYTE *)pVTrArgs);
        return 0;
    }
    return threadID;
#endif
}



/******************************************************************************

 *Name        : vos_ThreadDelete

 *Function    :  delete a thread

 *para        :

 *return      :   if 0 OK, != 0 error

 *Created by Lisy 2013/6/8
 ******************************************************************************/
int vos_ThreadDelete(VOS_THREAD_ID threadID)
{
    int result;
#if defined(LINUX)
    result = pthread_cancel(threadID);
    if(0 != result)
    {
        VOS_PRINT1(VOS_ERR, "vos_ThreadSpawn pthread_cancel failed Linux return=%d\n", result);
        result = (int)RET_ERR;
    }
    else
    {
        result = (int)RET_OK;
    }
#elif defined(VXWORKS)
    char errBuf[NAME_MAX];
    result = taskDelete(threadID);
    if(OK != result)
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "taskDelete failed. VxWorks errno=%#x %s\n", errno, errBuf);
        result = (int)RET_ERR;
    }
    else
    {
        result = (int)RET_OK;
    }
#endif
    return result;
}



/******************************************************************************

 *Name        : vos_CreateSem

 *Function    :  Create a sempahore, use mutual semaphore.
 *               VOS_SEM_Q_PRIORITY | SEM_INVERSION_SAFE

 *para        :

 *return      :

 *Created by Lisy 2012/7/18
 ******************************************************************************/
int vos_CreateSem
(
    VOS_SEM *pSem,                /* pointer to semaphore handle */
    VOS_SEM_STATE initialState        /* The initial state of the sempahore */
)
{
#if defined(LINUX)
#ifdef LINUX_MULTIPROC      /* Process semaphores for multi process environment */
    int ret;
    union semun
    {
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
    } arg;
    unsigned short val[1];
    char errBuf[NAME_MAX];
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_CreateSem: Wrong parameter pSem = NULL\n");
        return((int)VOS_SEM_ERROR);
    }
    ret = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
    if (ret == -1)
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_CreateSem: semget Linux errno=%d %s\n", errno, errBuf);
        *pSem = (VOS_SEM) NULL;
        return (int)VOS_SEM_ERROR;
    }
    else
    {
        *pSem = ret;
    }
    /* Set semaphore state to full or empty */
    val[0] = (initialState == VOS_SEM_FULL) ? 1 : 0;
    arg.array = val;
    ret = semctl(*pSem, 0, SETALL, arg);
    if (ret == -1)
    {
        (void)strerror_r(errno, errBuf,sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_CreateSem: semctl Linux errno=%d %s\n", errno, errBuf);
        *pSem = (VOS_SEM) NULL;
        return (int)VOS_SEM_ERROR;
    }
    return RET_OK;
#else
    int RetCode;
    char errBuf[NAME_MAX];
    pthread_condattr_t condAttr;
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_CreateSem: Wrong parameter pSem=NULL\n");
        return((int)VOS_SEM_ERROR);
    }
    RetCode = pthread_mutex_init(&(pSem->mutex), NULL);
    if(RetCode != 0)
    {
        (void)strerror_r(errno, errBuf,sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_CreateSem: pthread_mutex_init Linux errno=%d %s\n", errno, errBuf);
        return (int)VOS_SEM_ERROR;
    }
    RetCode = pthread_condattr_init(&condAttr);
    if(RetCode != 0)
    {
        (void)strerror_r(errno, errBuf,sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_CreateSem: pthread_condattr_init Linux errno=%d %s\n", errno, errBuf);
        (void)pthread_mutex_destroy( &(pSem->mutex) );
        return (int)VOS_SEM_ERROR;
    }
#ifdef SETCLOCK_SUPPORT
    RetCode = pthread_condattr_setclock(&condAttr, CLOCK_MONOTONIC);
    if(RetCode != 0)
    {
        (void)strerror_r(errno, errBuf,sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_CreateSem: pthread_condattr_setclock Linux errno=%d %s\n", errno, errBuf);
        (void)pthread_mutex_destroy( &(pSem->mutex) );
        return (int)VOS_SEM_ERROR;
    }
#endif
    RetCode = pthread_cond_init(&(pSem->condition), &condAttr);
    if(RetCode != 0)
    {
        (void)strerror_r(errno, errBuf,sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_CreateSem: pthread_cond_init Linux errno=%d %s\n", errno, errBuf);
        (void)pthread_mutex_destroy( &(pSem->mutex) );
        return (int)VOS_SEM_ERROR;
    }
    if (initialState == VOS_SEM_FULL)
    {
        pSem->semCount = 1;
    }
    else
    {
        pSem->semCount = 0;
    }
    return (int)RET_OK;
#endif
#elif defined(VXWORKS)
    char errBuf[NAME_MAX];
    if(NULL == pSem)
    {
        VOS_PRINT0(VOS_ERR, "vos_CreateSem: Wrong parameter pSem=NULL\n");
        return (int)VOS_SEM_ERROR;
    }
    (*pSem)=semMCreate(VOS_SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
    if(NULL != (*pSem))
    {
        if(VOS_SEM_FULL != initialState)
        {
            if(OK != semTake((SEM_ID)*pSem, VOS_NO_WAIT))
            {
                return (int)VOS_SEM_ERROR;
            }
        }
        return (int)RET_OK;
    }
    else
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_CreateSem failed: semMCreate VxWorks errno=%#x %s\n", errno, errBuf);
        return (int)VOS_SEM_ERROR;
    }
#endif
}



/******************************************************************************

 *Name        : vos_GetSem

 *Function    :  take a semaphore.
 *

 *para        :

 *return      :

 *Created by Lisy 2012/7/20
 ******************************************************************************/
int vos_GetSem
(
    VOS_SEM *pSem,         /* pointer to semaphore handle */
    int timeoutInMs         /* timeout time in milliseconds */
)
{
#if defined(LINUX)
#ifdef LINUX_MULTIPROC
    struct sembuf op[1];
    int ret = 1;
    char errBuf[NAME_MAX];
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_GetSem: Wrong parameter pSem=NULL\n");
        return((int)VOS_SEM_ERROR);
    }
    op[0].sem_num = 0;        /* Use first and only semaphore */
    op[0].sem_op = -1;        /* Decrement by 1 */
    op[0].sem_flg = SEM_UNDO;   /* Permit undoing */
    if (timeoutInMs == VOS_WAIT_FOREVER)
    {
        while (1)
        {
            /* Wait forever */
            ret = semop(*pSem, op, 1);
            if (ret == 0)
            {
                /* OK */
                break;
            }
            else if (errno != EINTR)
            {
                (void)strerror_r(errno, errBuf,sizeof(errBuf));
                VOS_PRINT2(VOS_ERR, "vos_GetSem: semop Linux errno=%d %s\n", errno, errBuf);
                break;
            }
        }
    }
    else
    {
        /* Return immediately */
        if (timeoutInMs == VOS_NO_WAIT)
        {
            op[0].sem_flg |= IPC_NOWAIT;
            while (1)
            {
                ret = semop(*pSem, op, 1);
                if ((ret == 0) || (errno == EAGAIN ))
                {
                    break;
                }
                else if (errno != EINTR)
                {
                    (void)strerror_r(errno, errBuf, sizeof(errBuf));
                    VOS_PRINT2(VOS_ERR, "vos_GetSem: semop Linux errno=%d %s\n", errno, errBuf);
                    break;
                }
            }
        }
        else
        {
#if 0
            struct timespec timeOut;
            /* Wait specified time */
            timeOut.tv_sec = timeOutInMs / 1000;
            timeOut.tv_nsec = (long) 1000000 * (timeOutInMs % 1000);
            ret = semtimedop(*hSem, op, 1, &timeOut);
#else
            VOS_PRINT0(VOS_ERR, "vos_GetSem with timeout in ms not supported\n");
#endif
        }
    }
    return (ret == 0) ? (int)RET_OK : (int)VOS_SEM_ERROR;
#else
    struct timespec delay;
    int RetCode;
    int timeout;
    char errBuf[NAME_MAX];
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_GetSem: Wrong parameter pSem=NULL\n");
        return((int)VOS_SEM_ERROR);
    }
    /* Wait forever */
    if (timeoutInMs == VOS_WAIT_FOREVER)
    {
        RetCode = pthread_mutex_lock(&(pSem->mutex));
        if (RetCode != 0)
        {
            (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
            VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_mutex_lock Linux return=%d %s\n", RetCode, errBuf);
            return (int)VOS_SEM_ERROR;
        }
    }
    else
    {
        /* Return immediately */
        if (timeoutInMs == VOS_NO_WAIT)
        {
            RetCode = pthread_mutex_trylock(&(pSem->mutex));
            if (RetCode != 0)
            {
                if (RetCode != EBUSY)
                {
                    (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
                    VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_mutex_trylock Linux return=%d %s\n", RetCode, errBuf);
                }
                return (int)VOS_SEM_ERROR;
            }
        }
        else
        {
            /* Wait with timeout */
            timeout = 0;
            do
            {
                delay.tv_sec = 0;
                delay.tv_nsec = 1000000;  /* 1 milli sec */
                RetCode = pthread_mutex_trylock(&(pSem->mutex));
                if (RetCode == 0)
                {
                    /* we now own the mutex  */
                    break;
                }
                else
                {
                    /* Did somebody else own the semaphore */
                    if (RetCode == EPERM )
                    {
                        /* Wait a millisec */
                        (void)nanosleep(&delay, NULL);
                        timeout++ ;
                    }
                    else
                    {
                        (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
                        VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_mutex_trylock Linux return=%d %s\n", RetCode, errBuf);
                        /* Other error  */
                        return (int)VOS_SEM_ERROR;
                    }
                }
            }
            while (timeout < timeoutInMs);
        }
    }
    while (pSem->semCount <= 0)
    {
        if(timeoutInMs == VOS_WAIT_FOREVER)
        {
            RetCode = pthread_cond_wait(&(pSem->condition), &(pSem->mutex));
            if ((RetCode!=0) &&( errno != EINTR) )
            {
                (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
                VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_cond_wait Linux return=%d %s\n", RetCode, errBuf);
                break;
            }
        }
        else
        {
            struct timespec timeoutValue;
#ifdef SETCLOCK_SUPPORT
            if(0 != syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &timeoutValue))
            {
                VOS_PRINT1(VOS_ERR, "failed: clock_gettime Linux errno=%#x\n", errno);
                return (int)VOS_SEM_ERROR;
            }
#else
            if(0 != syscall(__NR_clock_gettime, CLOCK_REALTIME, &timeoutValue))
            {
                VOS_PRINT1(VOS_ERR, "failed: clock_gettime Linux errno=%#x\n", errno);
                return (int)VOS_SEM_ERROR;
            }
#endif
            /*the second should be div 1000*/
            timeoutValue.tv_sec += timeoutInMs / 1000;
            timeoutValue.tv_nsec += (timeoutInMs % 1000) * (long)1000000;
            RetCode = pthread_cond_timedwait(&(pSem->condition), &(pSem->mutex), &timeoutValue);
            if(RetCode == ETIMEDOUT)
            {
                RetCode = pthread_mutex_unlock(&(pSem->mutex));
                if (RetCode != 0)
                {
                    (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
                    VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_mutex_unlock Linux return=%d %s\n", RetCode, errBuf);
                }
                return (int)VOS_SEM_ERROR;
            }
            else if (errno != EINTR)
            {
                (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
                VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_cond_timedwait Linux return=%d %s\n", RetCode, errBuf);
                break;
            }
        }
    }
    pSem->semCount--;
    RetCode = pthread_mutex_unlock(&(pSem->mutex));
    if (RetCode != 0)
    {
        (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_GetSem: pthread_mutex_unlock Linux return=%d %s\n", RetCode, errBuf);
        return (int)VOS_SEM_ERROR;
    }
    return (int)RET_OK;
#endif
#elif defined(VXWORKS)
    int noTicks;
    int clockRate;
    if(NULL == pSem)
    {
        VOS_PRINT0(VOS_ERR, "vos_GetSem: wrong parameter pSem=NULL\n");
        return (int)VOS_SEM_ERROR;
    }
    if(VOS_NO_WAIT == timeoutInMs)
    {
        noTicks = VOS_NO_WAIT;
    }
    else if(VOS_WAIT_FOREVER == timeoutInMs)
    {
        noTicks = VOS_WAIT_FOREVER;
    }
    else
    {
        clockRate = sysClkRateGet();
        noTicks = (clockRate * timeoutInMs + 999) / 1000;
    }
    if(OK == semTake(((SEM_ID)*pSem), noTicks))
    {
        return (int)RET_OK;
    }
    else
    {
        return (int)VOS_SEM_ERROR;
    }
#endif
}



/******************************************************************************

 *Name        : vos_PutSemR

 *Function    :  give a semaphore.
 *

 *para        :

 *return      :   RET_OK on success, VOS_SEM_ERR otherwise

 *Created by Lisy 2012/7/20
 ******************************************************************************/
int vos_PutSemR
(
    VOS_SEM *pSem                  /* pointer to semaphore handle */
)
{
#if defined(LINUX)
    int RetCode;
#ifdef LINUX_MULTIPROC
    struct sembuf op[1];
    char errBuf[NAME_MAX];
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_PutSemR: Wrong parameter pSem=NULL\n");
        return((int)VOS_SEM_ERROR);
    }
    op[0].sem_num = 0;        /* Use first and only semaphore */
    op[0].sem_op = 1;         /* Increment by 1 */
    op[0].sem_flg = SEM_UNDO;  /* Permit undoing */
    while (1)
    {
        RetCode = semop(*pSem, op, 1);
        if (RetCode == 0)
        {
            break;
        }
        else if (errno != EINTR)
        {
            (void)strerror_r(errno, errBuf, sizeof(errBuf));
            VOS_PRINT2(VOS_ERR, "vos_PutSemR: semop Linux errno=%d %s\n", errno, errBuf);
            return((int)VOS_SEM_ERROR);
        }
    }
    return((int)RET_OK);
#else
    char errBuf[NAME_MAX];
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_PutSemR: Wrong parameter pSem=NULL\n");
        return((int)VOS_SEM_ERROR);
    }
    RetCode = pthread_mutex_lock(&(pSem->mutex));
    if (RetCode != 0)
    {
        (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_PutSemR: pthread_mutex_lock Linux return=%d %s\n", RetCode, errBuf);
        return((int)VOS_SEM_ERROR);
    }
    if (pSem->semCount < 1)
    {
        pSem->semCount++;
    }
    RetCode = pthread_mutex_unlock(&(pSem->mutex));
    if (RetCode != 0)
    {
        (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_PutSemR: pthread_mutex_unlock Linux return=%d %s\n", RetCode, errBuf);
        return((int)VOS_SEM_ERROR);
    }
    RetCode = pthread_cond_signal(&(pSem->condition));
    if (RetCode != 0)
    {
        (void)strerror_r(RetCode, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_PutSemR: pthread_cond_signal Linux return=%d %s\n", RetCode, errBuf);
        return((int)VOS_SEM_ERROR);
    }
    return((int)RET_OK);
#endif
#elif defined(VXWORKS)
    char errBuf[NAME_MAX];
    if(NULL == pSem)
    {
        VOS_PRINT0(VOS_ERR, "vos_GetSem: wrong parameter pSem=NULL\n");
        return (int)VOS_SEM_ERROR;
    }
    if(ERROR == semGive((SEM_ID)*pSem))
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_PutSem failed: semGive vxWorks errno=%#x, %s\n",
                   errno, errBuf);
        return (int)VOS_SEM_ERROR;
    }
    return (int)RET_OK;
#endif
}



/******************************************************************************

 *Name        : vos_PutSem

 *Function    :  give a semaphore.
 *

 *para        :

 *return      :   -

 *Created by Lisy 2012/7/20
 ******************************************************************************/
int vos_PutSem
(
    VOS_SEM *pSem                  /* pointer to semaphore handle */
)
{
    return vos_PutSemR(pSem);
}



/******************************************************************************

 *Name        : vos_DestroySem

 *Function    :  delete a sempahore

 *para        :

 *return      :

 *Created by Lisy 2012/7/18
 ******************************************************************************/
void vos_DestroySem
(
    VOS_SEM *pSem                /* pointer to semaphore handle */
)
{
#if defined(LINUX)
    int res;
#ifdef LINUX_MULTIPROC
    union semun
    {
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
    } arg;
    char errBuf[NAME_MAX];
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_DestroySem: Wrong parameter pSem=NULL\n");
        return;
    }
    res = semctl(*pSem, 1, IPC_RMID, arg);  /*lint !e530 Ignore warning, arg is not used */
    if (res < 0)
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_DestroySem: semctl Linux errno=%d %s\n", errno, errBuf);
    }
#else
    if (pSem == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_DestroySem: Wrong parameter pSem=NULL\n");
        return;
    }
    res = pthread_mutex_destroy(&(pSem->mutex));
    if (res != 0)
    {
        VOS_PRINT1(VOS_ERR, "vos_DestroySem: pthread_mutex_destroy Linux return=%d\n", res);
    }
#endif
#elif defined(VXWORKS)
    char errBuf[NAME_MAX];
    if(NULL == pSem)
    {
        VOS_PRINT0(VOS_ERR, "vos_DestroySem: Wrong parameter pSem=NULL\n");
        return;
    }
    if(ERROR == semDelete((SEM_ID)*pSem))
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_DestroySem failed: semDelete VxWorks errno=%#x %s\n",errno, errBuf);
        return;
    }
#endif
}



/******************************************************************************

 *Name        : vos_CreateMsgQueue

 *Function    :  Create a message queue. VOS_MSG_Q_FIFO

 *para        :

 *return      :

 *Created by Lisy 2012/7/18
 ******************************************************************************/
int vos_CreateMsgQueue
(
    VOS_QUEUE *pQueueId,    /* pointer to queue */
    int maxNoMsg,            /* max messages that can be queued */
    int maxLength            /* max bytes in a message */
)
{
#if defined(LINUX)
    int  i;
    int  qid;
    key_t keyval;
    FILE  *fp;
    int msgmax;
    int msgmnb;
    char errBuf[NAME_MAX];
    if (pQueueId == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_CreateMsgQueue: Wrong parameter pQueueId=NULL\n");
        return((int)VOS_QUEUE_ERROR);
    }
    /* Check parameters */
    fp = fopen("/proc/sys/kernel/msgmax","r");
    if (fp)
    {
        if(fscanf(fp, "%d", &msgmax) <= 0)
        {
            VOS_PRINT0(VOS_ERR, "can not get parameter:msgmax\n");
            return(int)RET_ERR;
        }
        if(fclose(fp))
        {
            VOS_PRINT0(VOS_ERR, "close /proc/sys/kernel/msgmax file failed\n");
            return(int)RET_ERR;
        }
        if (maxLength > msgmax)
        {
            VOS_PRINT2(VOS_ERR, "A queue couldn't be created. Max msg size=%d > msgmax=%d\n", maxLength, msgmax);
            *pQueueId = 0;
            VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32)++;
            return((int)VOS_QUEUE_ERROR);
        }
    }
    fp = fopen("/proc/sys/kernel/msgmnb","r");
    if (fp)
    {
        if(fscanf(fp, "%d", &msgmnb) <= 0)
        {
            VOS_PRINT0(VOS_ERR, "can not get parameter:msgmnb\n");
            return(int)RET_ERR;
        }
        if(fclose(fp))
        {
            VOS_PRINT0(VOS_ERR, "close /proc/sys/kernel/msgmnb file failed\n");
            return(int)RET_ERR;
        }
        if ((maxNoMsg * maxLength) > msgmnb)
        {
            if (maxLength != 0)
            {
                /*th message nums is message bytes nums div massage length*/
                VOS_PRINT2(VOS_ERR, "A queue couldn't be created. Max msg no=%d > System limit=%d\n", maxNoMsg, msgmnb/maxLength);
            }
            else
            {
                VOS_PRINT3(VOS_ERR, "A queue couldn't be created. Max msg no=%d, Msgnb=%d, Maxlength=%d\n", maxNoMsg, msgmnb, maxLength);
            }
            *pQueueId = 0;
            VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32)++;
            return((int)VOS_QUEUE_ERROR);
        }
    }
    i = 0;
    /* Try to create a free queue */
    do
    {
        VOSGLOBAL(QueueNo_U32)++;
        /* The least significant bit must not be zero when ftok is called */
        if ((VOSGLOBAL(QueueNo_U32) & 0xff) == 0)
        {
            VOSGLOBAL(QueueNo_U32)++;
        }
        /* Create unique queue key using file id */
        keyval = ftok(linuxFile, VOSGLOBAL(QueueNo_U32));
        if (keyval == -1)
        {
            (void)strerror_r(errno, errBuf, sizeof(errBuf));
            VOS_PRINT2(VOS_ERR, "A queue couldn't be created. ftok Linux errno=%d %s\n", errno, errBuf);
            *pQueueId = 0;
            VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32)++;
            return((int)VOS_QUEUE_ERROR);
        }
        qid = msgget(keyval, IPC_CREAT | IPC_EXCL | 0660);
        /* Avoid qid == 0, some Linux versions returns zero */
        if (qid == 0)
        {
            /* Destroy the queue with qid = 0 */
            (void)vos_DestroyMsgQueue(&qid);
        }
        else if(qid != -1)
        {
            *pQueueId = qid;
            VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32)++;
            if (VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32) > VOSGLOBAL(QueueCnt_ST.QueueMax_U32))
            {
                VOSGLOBAL(QueueCnt_ST.QueueMax_U32) = VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32);
            }
            /* A queue is created */
            return((int)RET_OK);
        }
        else
        {
            /* Error not equal to "A message queue for the keyval already exists" */
            if (errno != EEXIST)
            {
                (void)strerror_r(errno, errBuf, sizeof(errBuf));
                VOS_PRINT2(VOS_ERR, "A queue couldn't be created. msgget Linux errno=%d %s\n", errno, errBuf);
                *pQueueId = 0;
                VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32)++;
                return((int)VOS_QUEUE_ERROR);
            }
        }
        /* Try next keyval */
        i++;
    }
    while(i < 256); /* More possible keyval? */
    /* No free queue available */
    VOS_PRINT0(VOS_ERR, "A queue couldn't be created, no free queues \n");
    *pQueueId = 0;
    VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32)++;
    return((int)VOS_QUEUE_ERROR);
#elif defined(VXWORKS)
    MSG_Q_ID qid = NULL;
    char errBuf[NAME_MAX];
    if(NULL == pQueueId)
    {
        VOS_PRINT0(VOS_ERR, "vos_CreateMsgQueue: Wrong parameter pQueueId=NULL\n");
        return (int)VOS_QUEUE_ERROR;
    }
    qid = msgQCreate(maxNoMsg, maxLength, VOS_MSG_Q_FIFO);
    if(NULL != qid)
    {
        *pQueueId = qid;
        VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32)++;
        if(VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32) > VOSGLOBAL(QueueCnt_ST.QueueMax_U32))
        {
            VOSGLOBAL(QueueCnt_ST.QueueMax_U32)=VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32);
        }
        return (int)RET_OK;
    }
    else
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_CreateMsgQueue failed: msgQCreate VxWorks errno=%#x %s\n",errno, errBuf);
        pQueueId = NULL;
        VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32)++;
        return (int)VOS_QUEUE_ERROR;
    }
#endif
}



/******************************************************************************

 *Name        : vos_DestroyMsgQueue

 *Function    :  delete a message queue.

 *para        :

 *return      :

 *Created by Lisy 2012/7/24
 ******************************************************************************/
int vos_DestroyMsgQueue
(
    VOS_QUEUE *pQueueId                          /* pointer to queue ID */
)
{
#if defined(LINUX)
    char errBuf[NAME_MAX];
    if (pQueueId == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_DestroyMsgQueue: Wrong parameter pQueueId=NULL\n");
        return((int)VOS_QUEUE_ERROR);
    }
    if (msgctl(*pQueueId, IPC_RMID, 0) == 0)
    {
        VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32)--;
        return((int)RET_OK);
    }
    else
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "Queue couldn't be deleted. msgctl Linux errno=%d %s\n", errno, errBuf);
        VOSGLOBAL(QueueCnt_ST.QueueDestroyErrCnt_U32)++;
        return((int)VOS_QUEUE_ERROR);
    }
#elif defined(VXWORKS)
    char errBuf[NAME_MAX];
    if(NULL == pQueueId)
    {
        VOS_PRINT0(VOS_ERR, "vos_DestroyMsgQueue: Wrong parameter pQueueId=NULL\n");
        return (int)VOS_QUEUE_ERROR;
    }
    if(OK == msgQDelete((MSG_Q_ID)*pQueueId))
    {
        VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32)--;
        return (int)RET_OK;
    }
    else
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_DestroyMsgQueue failed: msgQDelete VxWorks errno=%#x %s\n",errno, errBuf);
        VOSGLOBAL(QueueCnt_ST.QueueDestroyErrCnt_U32)++;
        return (int)VOS_QUEUE_ERROR;
    }
#endif
}



/******************************************************************************

 *Name        : vos_SendMsgQueue

 *Function    :  send a message on a queue. (NO_WAIT and MSG_PRI_NORMAL)

 *para        :

 *return      :

 *Created by Lisy 2012/7/24
 ******************************************************************************/
int vos_SendMsgQueue
(
    VOS_QUEUE *pQueueId,         /* pointer to queue */
    char *pMsg,                   /* message buf */
    UINT32 size                    /* size of message buf */
)
{
#if defined(LINUX)
    int res;
    struct msgbuf *pMsgBuf;
    char errBuf[NAME_MAX];
    if (pQueueId == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_SendMsgQueue: Wrong parameter pQueueId=NULL\n");
        return((int)VOS_QUEUE_ERROR);
    }
    pMsgBuf = (struct msgbuf *)pMsg; /*lint !e826 pMsg actually points to struct msgbuf*/
    pMsgBuf->mtype = VOS_MSG_BUF_TYPE;
    while (1)
    {
        res = msgsnd(*pQueueId, pMsgBuf, size - sizeof(long), IPC_NOWAIT);
        if (res == 0)
        {
            return((int)RET_OK);
        }
        else if (errno != EINTR)
        {
            (void)strerror_r(errno, errBuf, sizeof(errBuf));
            VOS_PRINT2(VOS_ERR, "vos_SendMsgQueue: msgsnd Linux errno=%d %s\n", errno, errBuf);
            VOSGLOBAL(QueueCnt_ST.QueueWriteErrCnt_U32)++;
            return((int)VOS_QUEUE_ERROR);
        }
    }
#elif defined(VXWORKS)
    int retCode;
    char errBuf[NAME_MAX];
    if(NULL == pQueueId)
    {
        VOS_PRINT0(VOS_ERR, "vos_SendMsgQueue: Wrong parameter pQueueId=NULL\n");
        return (int)VOS_QUEUE_ERROR;
    }
    retCode = msgQSend((MSG_Q_ID)*pQueueId, pMsg, size, VOS_NO_WAIT, MSG_PRI_NORMAL);
    if(OK == retCode)
    {
        return (int)RET_OK;
    }
    else
    {
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_SendMsgQueue failed: msgQSend VxWorks errno=%#x %s\n",errno, errBuf);
        VOSGLOBAL(QueueCnt_ST.QueueWriteErrCnt_U32)++;
        return (int)VOS_QUEUE_ERROR;
    }
#endif
}



/******************************************************************************

 *Name        : vos_ReceiveMsgQueue

 *Function    :  receive a message from a queue

 *para        :

 *return      :   number of bytes read if success, VOS_QUEUE_ERROR otherwise

 *Created by Lisy 2012/7/24
 ******************************************************************************/
int vos_ReceiveMsgQueue
(
    VOS_QUEUE *pQueueId,      /* pointer to queue */
    char *pMsg,               /* message buf */
    UINT32 size,               /* length of message buffer */
    int timeout                /* timeout */
)
{
#if defined(LINUX)
    int res;
    int msgFlg;
    struct msgbuf *pMsgBuf;
    char errBuf[NAME_MAX];
    if (pQueueId == NULL)
    {
        VOS_PRINT0(VOS_ERR, "vos_ReceiveMsgQueue: Wrong parameter pQueueId=NULL\n");
        return((int)VOS_QUEUE_ERROR);
    }
    pMsgBuf = (struct msgbuf *)pMsg;  /*lint !e826 pMsg actually points to struct msgbuf*/
    if (timeout == VOS_WAIT_FOREVER)
    {
        msgFlg = 0;
    }
    else if (timeout == VOS_NO_WAIT)
    {
        msgFlg = IPC_NOWAIT;
    }
    else
    {
        VOS_PRINT1(VOS_ERR, "vos_ReceiveMsgQueue wrong timeout value=%d\n", timeout);
        return (int)VOS_INVALID_PAR;
    }
    pMsgBuf->mtype = VOS_MSG_BUF_TYPE;
    while (1)
    {
        res = msgrcv(*pQueueId, (struct msgbuf *)pMsg, size - sizeof(long), pMsgBuf->mtype, msgFlg); /*lint !e826 pMsg actually points to struct msgbuf*/
        if (res > 0)
        {
            return(res + sizeof(long));
        }
        else
        {
            /*
             if WAIT_FOREVER then the queue id has been deleted by another thread, errno will set to EIDRM
            */
            if ((errno == ENOMSG) || (errno == EIDRM))
            {
                return(0);
            }
            else if (errno != EINTR)
            {
                (void)strerror_r(errno, errBuf, sizeof(errBuf));
                VOS_PRINT2(VOS_ERR, "vos_ReceiveMsgQueue: msgrcv Linux errno=%d %s\n", errno, errBuf);
                VOSGLOBAL(QueueCnt_ST.QueueReadErrCnt_U32)++;
                return((int)VOS_QUEUE_ERROR);
            }
        }
    }
#elif defined(VXWORKS)
    int result;
    char errBuf[NAME_MAX];
    if(NULL == pQueueId)
    {
        VOS_PRINT0(VOS_ERR, "vos_ReceiveMsgQueue: Wrong parameter pQueueId=NULL\n");
        return (int)VOS_QUEUE_ERROR;
    }
    if((VOS_NO_WAIT != timeout) && (VOS_WAIT_FOREVER != timeout))
    {
        VOS_PRINT1(VOS_ERR, "vos_ReceiveMsgQueue: Wrong timeout value=%d\n, only can \
		be set to NO_WAIT or WAIT_FOREVER\n", timeout);
        return (int)VOS_INVALID_PAR;
    }
    result = msgQReceive((MSG_Q_ID)*pQueueId, pMsg, size, timeout);
    if(result >= 0)
    {
        return result;
    }
    else
    {
        /* NO_WAIT means not need wait, if CANNOT call, errno will set S_objLib_OBJ_UNAVAILABLE,
         * if a positive(>0) time to wait and CANNOT call after timeout, errno will set S_objLib_OBJ_TIMEOUT
         * if WAIT_FOREVER then the queue id has been deleted, errno will set S_objLib_OBJ_DELETED */
        if((S_objLib_OBJ_UNAVAILABLE == errno) || (S_objLib_OBJ_TIMEOUT == errno) ||
           (S_objLib_OBJ_DELETED == errno))
        {
            return 0;
        }
        else
        {
            (void)strerror_r(errno, errBuf);
            VOS_PRINT2(VOS_ERR, "vos_ReceiveMsgQueue failed: msgQReceive VxWorks errno=%#x %s\n",errno, errBuf);
            VOSGLOBAL(QueueCnt_ST.QueueReadErrCnt_U32)++;
            return (int)VOS_QUEUE_ERROR;
        }
    }
#endif
}




/******************************************************************************

 *Name        : vos_QueueShow

 *Function    :  display the queue statistic

 *para        :

 *return      :

 *Created by Lisy 2012/7/24
 ******************************************************************************/
void vos_QueueShow(void)
{
    VOS_PRINT0(VOS_DBG_INFO, "=============VOS QUEUE SHOW=============");
    VOS_PRINT1(VOS_DBG_INFO, "====Number of allocated queues = %d", VOSGLOBAL(QueueCnt_ST.QueueAllocated_U32));
    VOS_PRINT1(VOS_DBG_INFO, "====Maximum number of allocated queues = %d", VOSGLOBAL(QueueCnt_ST.QueueMax_U32));
    VOS_PRINT1(VOS_DBG_INFO, "====Number of queue create errors = %d", VOSGLOBAL(QueueCnt_ST.QueueCreateErrCnt_U32));
    VOS_PRINT1(VOS_DBG_INFO, "====Number of queue destroy errors = %d", VOSGLOBAL(QueueCnt_ST.QueueDestroyErrCnt_U32));
    VOS_PRINT1(VOS_DBG_INFO, "====Number of queue write errors = %d", VOSGLOBAL(QueueCnt_ST.QueueWriteErrCnt_U32));
    VOS_PRINT1(VOS_DBG_INFO, "====Number of queue read errors = %d", VOSGLOBAL(QueueCnt_ST.QueueReadErrCnt_U32));
}



/******************************************************************************

 *Name        : vos_DateTimeString

 *Function    : get date and time string

 *para        :

 *return      :

 *Created by Lisy 2012/7/17

 ******************************************************************************/
void vos_DateTimeString
(
    char *pString,         /* output:pointer to date time string */
    UINT32 bufLen           /* buffer length */
)
{
#if defined(LINUX)
    struct timeval tv;
    struct timezone tz;
    struct tm ltm;
    char errBuf[NAME_MAX];
    if (gettimeofday(&tv, &tz) == 0)
    {
        (void)localtime_r(&tv.tv_sec, &ltm);    /* convert to local time */
        sprintf(pString,"%04d-%02d-%02d %02d:%02d:%02d:%03d",
                1900+(ltm.tm_year), 1+(ltm.tm_mon), ltm.tm_mday,
                ltm.tm_hour, ltm.tm_min, ltm.tm_sec, (int)(tv.tv_usec/1000));
    }
    else
    {
        sprintf(pString," ");
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_DateTimeString: gettimeofday Linux errno=%d %s\n", errno, errBuf);
    }
#elif defined(VXWORKS)
    struct timespec curTime;
    struct tm locTime;
    UINT32 timestamp;
    char errBuf[NAME_MAX];
    int lvl;
    UINT32 ticSubPart;
    lvl=intLock();
    /* get the current time of the clock
     * CLOCK_REALTIME: system wide realtime clock, time-of-day time */
    if(ERROR == clock_gettime(CLOCK_REALTIME, &curTime))
    {
        (void)snprintf(pString, bufLen, " ");
        (void)strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_DateTimeString failed: clock_gettime VxWorks errno=%#x %s\n",
                   errno, errBuf);
        (void)intUnlock(lvl);
    }
    else
    {
        timestamp=sysTimestamp();
        (void)intUnlock(lvl);
        ticSubPart=(timestamp*1000+timeStampFreq/2000000)/(timeStampFreq/1000000);
        /* convert to local time , localtime_r convert the calendar time into
         * broken-down time, expressed as local time */
        (void)localtime_r(&curTime.tv_sec, &locTime);
        (void)snprintf(pString, bufLen, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
                       1900+(locTime.tm_year), 1+(locTime.tm_mon), locTime.tm_mday,
                       locTime.tm_hour, locTime.tm_min, locTime.tm_sec,
                       (int)(curTime.tv_nsec+ticSubPart)/1000000);
    }
#endif
}



/******************************************************************************

 *Name        : vos_GetSecTimer

 *Function    : get second counter value  (absolute value according to CPU)

 *para        :

 *return      :

 *Created by Lisy 2012/7/17

 ******************************************************************************/
UINT32 vos_GetSecTimer(void)
{
#if defined(LINUX)
    struct timespec curtime;
    char errBuf[NAME_MAX];
#if defined(CCUC_PLATF)
    /* CLOCK_REALTIME is used as NRTOS doesn't support CLOCK_MONOTONIC,
      i.e. Clock setting will have impact on time measurments */
    if (clock_gettime(CLOCK_REALTIME, &curtime) != 0)
#elif defined(LINUX_ARM_OTN_PLATF)
    if (clock_gettime(CLOCK_REALTIME, &curtime) != 0)
#else
    /* syscall is used as clock_gettime is not included in HMI400 and HMI500 libraries */
    if (syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &curtime) != 0)
#endif
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_GetSecTimer failed: clock_gettime Linux errno=%#x %s\n", errno, errBuf);
        /* error */
        return 0;
    }
    return curtime.tv_sec;
#elif defined(VXWORKS)
    if(!VOSGLOBAL(ecspUsed))
    {
        struct timespec curTime;
        char errBuf[NAME_MAX];
        if(ERROR == clock_gettime(CLOCK_MONOTONIC, &curTime))
        {
            (void)strerror_r(errno, errBuf); /* map an error number to an error string */
            VOS_PRINT2(VOS_ERR, "vos_GetSecTimer failed: clock_gettime VxWorks errno=%#x %s\n",
                       errno, errBuf);
            return 0;
        }
        /* unsigned long is 32bit */
        return((UINT32)curTime.tv_sec);
    }
    else
    {
        UINT32 ticksPerSecond = sysClkRateGet();
        UINT32 osTicks = GetTickVal();
        UINT32 secs;
        secs = (osTicks + ticksPerSecond / 2) / ticksPerSecond;
        return secs;
    }
#endif
}



/******************************************************************************

 *Name        : vos_GetMilliSecTimer

 *Function    : get millisecond counter value  (absolute value according to CPU)

 *para        :

 *return      :

 *Created by Lisy 2012/7/16

 ******************************************************************************/
UINT32 vos_GetMilliSecTimer(void)
{
#if defined(LINUX)
    struct timespec curtime;
    char errBuf[NAME_MAX];
#if defined(CCUC_PLATF)
    /* CLOCK_REALTIME is used as NRTOS doesn't support CLOCK_MONOTONIC,
     i.e. Clock setting will have impact on time measurments */
    if (clock_gettime(CLOCK_REALTIME, &curtime) != 0)
#elif defined(LINUX_ARM_OTN_PLATF)
    if (clock_gettime(CLOCK_REALTIME, &curtime) != 0)
#else
    /* syscall is used as clock_gettime is not included in HMI400 and HMI500 libraries */
    if (syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &curtime) != 0)
#endif
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_GetSecTimer failed: clock_gettime Linux errno=%#x %s\n", errno, errBuf);
        /* error */
        return 0;
    }
    return ((curtime.tv_sec * 1000 )+( curtime.tv_nsec / 1000000));
#elif defined(VXWORKS)
#if 0
    if(!VOSGLOBAL(ecspUsed))
    {
        struct timespec curTime;
        UINT32 ticSubPart;
        UINT32 timestamp;
        UINT32 retValue;
        int lvl;
        char errBuf[NAME_MAX];
        lvl=intLock();   /* lock output interrupt */
        /* get the current time of the clock
         * CLOCK_MONOTONIC: system wide monotonic clock, absolute elapsed clock
         * time since system start up */
        if(ERROR == clock_gettime(CLOCK_MONOTONIC, &curTime))
        {
            (void)strerror_r(errno, errBuf); /* map an error number to an error string */
            VOS_PRINT2(VOS_ERR, "vos_GetMilliSecTimer failed: clock_gettime VxWorks errno=%#x %s\n",
                       errno, errBuf);
            (void)intUnlock(lvl);
            return 0;
        }
        /* get the timestamp timer tick counter, timestamp timer tick counter is
         * different from tick counter, tickGet() return the value of tick counter */
        timestamp = sysTimestamp();
        (void)intUnlock(lvl);
        /* compute how long we are into the tic period in ns (half adjusting)
         * sysTimestamp() can be converted to seconds by dividing by the return of
         * sysTimestampFreq() */
        ticSubPart = (timestamp * 1000 + timeStampFreq / 2000000) / (timeStampFreq / 1000000);
        /* get millisecond counter value (half adjusting) */
        retValue = curTime.tv_sec * 1000 + (curTime.tv_nsec + ticSubPart + 500000) / 1000000;
        return(retValue);
    }
    else
#endif
    {
        UINT32 ticksPerMillSecond = sysClkRateGet() / 1000;
        UINT32 osTicks = GetTickVal();
        UINT32 msecs;
        msecs = (osTicks + ticksPerMillSecond / 2) / ticksPerMillSecond;
        return msecs;
    }
#endif
}



/******************************************************************************

 *Name        : vos_GetMicroSecTimer

 *Function    : get microsecond counter value  (absolute value according to CPU)

 *para        :

 *return      :

 *Created by Lisy 2012/7/16

 ******************************************************************************/
UINT32 vos_GetMicroSecTimer(void)
{
#if defined(LINUX)
    struct timespec curtime;
    char errBuf[NAME_MAX];
#if defined(CCUC_PLATF)
    /* CLOCK_REALTIME is used as NRTOS doesn't support CLOCK_MONOTONIC,
     i.e. Clock setting will have impact on time measurments */
    if (clock_gettime(CLOCK_REALTIME, &curtime) != 0)
#elif defined(LINUX_ARM_OTN_PLATF)
    if (clock_gettime(CLOCK_REALTIME, &curtime) != 0)
#else
    /* syscall is used as clock_gettime is not included in HMI400 and HMI500 libraries */
    if (syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &curtime) != 0)
#endif
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR, "vos_GetSecTimer failed: clock_gettime Linux errno=%#x %s\n", errno, errBuf);
        /* error */
        return 0;
    }
    return ((curtime.tv_sec * 1000000 )+ (curtime.tv_nsec / 1000));
#elif defined(VXWORKS)
    struct timespec curTime;
    UINT32 timestamp;
    UINT32 ticSubPart;
    UINT32 retValue;
    int lvl;
    char errBuf[NAME_MAX];
    lvl=intLock();
    if(ERROR == clock_gettime(CLOCK_MONOTONIC, &curTime))
    {
        strerror_r(errno, errBuf);
        VOS_PRINT2(VOS_ERR, "vos_GetMicroSecTimer failed: \
		clock_gettime VxWorks errno=%#x %s\n", errno, errBuf);
        intUnlock(lvl);
        return 0;
    }
    timestamp = sysTimestamp();
    intUnlock(lvl);
    ticSubPart = (timestamp * 1000 + timeStampFreq / 2000000) / (timeStampFreq / 1000000);
    /* get microsecond counter value (half adjusting) */
    retValue = curTime.tv_sec * 1000000 + (curTime.tv_nsec + ticSubPart + 500) / 1000;
    return(retValue);
#endif
}



/******************************************************************************

 *Name        : vos_GetUniqueNumber

 *Function    : returns a number that is unique, 1..2^32

 *para        :

 *return      :

 *Created by Lisy 2012/7/24

 ******************************************************************************/
UINT32 vos_GetUniqueNumber(void)
{
    static int first = TRUE;
    static UINT32 n;
    if(first)
    {
        n = vos_GetMicroSecTimer();
        first = FALSE;
    }
    return n++;
}

#ifdef LINUX_MULTIPROC
/*******************************************************************************
NAME:      vos_CreateSharedMemory
ABSTRACT:  Create an area of shared memory to be used for globals in
           Linux multi-process systems.
RETURNS:   Pointer to memory if OK, NULL if not.
*/
char *vos_CreateSharedMemory
(
    UINT32 size    /* Size of shared memory area */
)
{
    int key, segId;
    VOS_SHARED_MEMORY_CB *pShm;
    char *pTop = (char *) 0x40000000;   /* Start address for Linux shared libraries */
    char errBuf[NAME_MAX];
    /* Create unique queue key using a file id */
    key = ftok(linuxFile, 1);
    if (key == -1)
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR,"vos_CreateSharedMemory FAILED. ftok Linux errno=%d %s\n", errno, errBuf);
        return NULL;
    }
    /* Allocate a shared memory segment.  */
    segId = shmget(key, size,
                   IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
    if (segId ==  -1)
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT3(VOS_ERR, "vos_CreateSharedMemory shmget FAILED size=%d Linux errno=%d %s\n", size, errno, errBuf);
        return NULL;
    }
    /* Attach to the shared memory segment, at address just below shared libraries. */
    pShm = (VOS_SHARED_MEMORY_CB *) shmat(segId, pTop - size, SHM_RND);
    if ((void *) pShm == (void *) -1)
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT4(VOS_ERR, "vos_CreateSharedMemory shmat FAILED, segId=%d, size=%d, Linux errno=%d %s\n", segId, size, errno, errBuf);
        return NULL;
    }
    /* Clear memory */
    memset(pShm, 0, size);
    /* Store segment ID and address first in global area */
    pShm->segId = segId;
    pShm->shmAddr = (char *) pShm;
    return (char *) pShm;
}



/*******************************************************************************
NAME:      vos_DestroySharedMemory
ABSTRACT:  Destroy an area of shared memory used for globals in
           Linux multi-process systems.
RETURNS:   -
*/
void vos_DestroySharedMemory
(
    char *pSharedMem
) /* Pointer to shared memory */
{
    int ret, segId;
    VOS_SHARED_MEMORY_CB *pShm;
    char errBuf[NAME_MAX];
    /* Get segment ID, should be found first in global area */
    pShm = (VOS_SHARED_MEMORY_CB *)((void *)pSharedMem);
    segId = pShm->segId;
    /* Detach the shared memory segment.  */
    ret = shmdt(pSharedMem);
    if (ret != 0)
    {
        (void)strerror_r(errno, errBuf, sizeof(errBuf));
        VOS_PRINT2(VOS_ERR,"shmdt failed Linux errno=%d %s\n", errno, errBuf);
    }
    else
    {
        /* Deallocate the shared memory segment.  */
        ret = shmctl(segId, IPC_RMID, 0);
        if (ret != 0)
        {
            (void)strerror_r(errno, errBuf, sizeof(errBuf));
            VOS_PRINT2(VOS_ERR,"shmctl failed Linux errno=%d %s\n", errno, errBuf);
        }
    }
}



/*******************************************************************************
NAME:      vos_AttachSharedMemory
ABSTRACT:  Attach to an area of shared memory used for globals in
           Linux multi-process systems. The area must have been created before
           by vos_CreateSharedMemory.
RETURNS:   Pointer to memory if OK, NULL if not.
*/
char *vos_AttachSharedMemory(void)
{
    INT32 Key_I32=0; /*the key value*/
    INT32 SegId_I32=0; /*the sem id value*/
    INT32 Ret_I32=0;  /*the return value*/
    VOS_SHARED_MEMORY_CB *Shm_pST=NULL;/**/
    void *NewShm_pV=NULL; /*the new share memory*/
    char ErrBuf_aC8[NAME_MAX]= {0}; /*the error buffer array*/
    /* Create unique queue key using a file id. */
    Key_I32 = ftok(linuxFile, 1);
    if (Key_I32 == -1)
    {
        (void)strerror_r(errno, ErrBuf_aC8, sizeof(ErrBuf_aC8));
        VOS_PRINT2(VOS_ERR,"vos_AttachSharedMemory FAILED. ftok Linux errno=%d %s\n", errno, ErrBuf_aC8);
        return NULL;
    }
    /* Get segment id  */
    SegId_I32 = shmget(Key_I32, 0, 0);
    if (SegId_I32 == -1)
    {
        (void)strerror_r(errno, ErrBuf_aC8, sizeof(ErrBuf_aC8));
        VOS_PRINT2(VOS_ERR,"vos_AttachSharedMemory FAILED. shmget Linux errno=%d %s\n", errno, ErrBuf_aC8);
        return NULL;
    }
    /* Attach to the shared memory segment.  */
    Shm_pST = (VOS_SHARED_MEMORY_CB *) shmat(SegId_I32, 0, 0);
    if ((void *) Shm_pST == (void *) -1)
    {
        (void)strerror_r(errno, ErrBuf_aC8, sizeof(ErrBuf_aC8));
        VOS_PRINT2(VOS_ERR,"vos_AttachSharedMemory FAILED. shmat Linux errno=%d %s\n", errno, ErrBuf_aC8);
        return NULL;
    }
    /* Get the address at which the primary process has located the shared memory */
    pNewShm = Shm_pST->shmAddr;
    /* Detach the shared memory and then reattach at the right address */
    Ret_I32 = shmdt((void *) Shm_pST);
    if (Ret_I32 != 0)
    {
        (void)strerror_r(errno, ErrBuf_aC8, sizeof(ErrBuf_aC8));
        VOS_PRINT2(VOS_ERR,"vos_AttachSharedMemory FAILED. shmdt Linux errno=%d %s\n", errno, ErrBuf_aC8);
        return NULL;
    }
    Shm_pST = (VOS_SHARED_MEMORY_CB *) shmat(SegId_I32, pNewShm, 0);
    if ((void *) Shm_pST== (void *) -1)
    {
        (void)strerror_r(errno, ErrBuf_aC8, sizeof(ErrBuf_aC8));
        VOS_PRINT2(VOS_ERR,"vos_AttachSharedMemory FAILED. shmat Linux errno=%d %s\n", errno, ErrBuf_aC8);
        return NULL;
    }
    return (char *) Shm_pST;
}




/******************************************************************************
 * Name:INT32 vos_DetachSharedMemory( char *SharedMem_pC8)
 * Function: detach the share memory..
 * Input:   SharedMem_pC8,Pointer to shared memory
 * return : ok,if the opearation successed.
 * Create by moyun  2014-7-23
 *******************************************************************************/
INT32 vos_DetachSharedMemory( char *SharedMem_pC8)
{
    INT32 Ret_I32=0; /*ther return value*/
    char ErrBuf_aC8[NAME_MAX]= {0}; /*error buffer */
    /* Detach the shared memory segment.  */
    Ret_I32 = shmdt(SharedMem_pC8);
    if (Ret_I32 != 0)
    {
        (void)strerror_r(errno,ErrBuf_aC8, sizeof(ErrBuf_aC8));
        VOS_PRINT2(VOS_ERR, "vos_DetachSharedMemory FAILED. shmdt Linux errno=%d %s\n",errno, ErrBuf_aC8);
        return (int)RET_ERR;
    }
    return RET_OK;
}
#endif


/******************************************************************************
 * Name:void vos_Initial(void)
 * Function: init the vos memory buffer.
 * Input:   void
 * return : ok,if the opearation successed.
 * Create by moyun  2014-7-23
 *******************************************************************************/
void vos_Initial(void)
{
    memset(&g_vos_ST, 0, sizeof(g_vos_ST));
    g_vos_ST.EcspUsed_B = TRUE;
}




