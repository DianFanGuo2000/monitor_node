/******************************************************************************

 *File Name    : vos.h

 *Copyright    : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.

 *Create Date  : 2012/3/14

 *Description  :

 *REV1.0.0     : lisy   2012/3/14  File Create


******************************************************************************/

/******************************************************************************
 *    Multi-Include-Prevent Start Section
 ******************************************************************************/
#ifndef VOS_LINUX_VXWORKS_H
#define VOS_LINUX_VXWORKS_H
/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/
#define LINUX
/******************************************************************************
*    Include File Section
******************************************************************************/
#if defined(LINUX)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sysctl.h>

#include <net/ethernet.h>
#include <net/if.h>


#include <netinet/in.h>

#include <sys/sysctl.h>

#include <linux/ioctl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/errno.h>
#define __USE_GNU
#include <sys/msg.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif
#include "mng_Interface.h"
#include "type.h"
//#include "vos_define.h"
#include "vos_Define.h"
#include "vos_Print.h"
#include "vos_com.h"
#include "vos_mem.h"
#include "vos_cmd.h"
/******************************************************************************
 *    Global Macro Define Section
 ******************************************************************************/
#define PACKED __attribute__((packed, aligned(1)))
/* linux */
#if defined(LINUX)
#define FAR
#define THREAD_METHOD

#define LINUX_FILE_SIZE  (20)

/* vxWorks */
#elif defined(VXWORKS)
#define FAR
#define THREAD_METHOD
#endif

/* trace output mask used together with setting trdpDebugMask and in calll of
 * trdpVosGetPrintMask and trdpVosSetPrintMask */
#define VOS_VOS         VOS_DBG_MEM   /* VOS */
#define VOS_NETSOCK     0x0002   /* netsock */
#define VOS_PD          0x0004  /* PD */
#define VOS_MD          0x0008  /* MD */
#define VOS_CONFIG     0x0010  /* configuration */
#define VOS_OTHER        0x0020  /* other */
#define VOS_WARN         VOS_DBG_WAR  /* warning */
#define VOS_ERR          VOS_DBG_ERR  /* error */
#define VOS_ALL          DBG_ALL  /* show everything */

#define VOS_UNUSED(x)    x=x

/* linux */
#if defined(LINUX)
#ifdef LINUX_MULTIPROC
typedef int VOS_SEM;
#else
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int semCount;
}  VOS_SEM;         /* semaphore definition */
#endif
typedef int VOS_QUEUE;
typedef pthread_t VOS_THREAD_ID; /* thread identity */
typedef void (*VOS_THREAD_ROUTINE)(void *pArg);

#define VOS_MSG_BUF_TYPE    0
#define VOS_MSG_NO_WAIT    1

/* vxWorks */
#elif defined(VXWORKS)
typedef void *VOS_SEM  ;     /* semaphore definition */
typedef void *VOS_QUEUE ;     /* queue definition */
typedef int VOS_THREAD_ID;      /* Thread identity */

/* pointer to thread routine, with arg of pointer to void  */
typedef void (*VOS_THREAD_ROUTINE)(void *pArg);
#endif

/* Maximum number of different sizes of memory allocation blocks*/
#define MEM_MAX_NBLOCKSIZES   100
/* default number of different sizes of memory allocation blocks */
#define MEM_NBLOCKSIZES       13
/* max blocks to pre-allocate */
#define MEM_MAX_PREALLOCATE   10
/* default sizes of memory blocks 32 to 128K */
#define VOS_MEM_BLOCKSIZES { 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072 }
/* default pre-allocation of free memory blocks. to avoid problem
 * with too many small blocks and no large. specify how many of each
 *  block size that should be pre-allocated (and freed) */
#define MEM_PREALLOCATE { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 4}

#define VOS_SEM_NO_WAIT          (0)
#define VOS_SEM_WAIT_FOREVER     (-1)
/******************************************************************************
 *    Global Structure Define Section
 ******************************************************************************/
typedef struct MemBlock
{
    UINT32 Size_U32;          /* size of the data part of the block */
    struct MemBlock *Next_pST;   /* pointer to next block in linked list */
    /* data area follows here */
} VOS_MEM_BLOCK;

typedef struct
{
    UINT32 FreeSize_U32;                         /* Size of free memory */
    UINT32 MinFreeSize_U32;                      /* min size of free memory  TO-ANALYSE */
    UINT32 AllocCnt_U32;                         /* No of allocated memory blocks */
    UINT32 AllocErrCnt_U32;                      /* No of allocated memory errors */
    UINT32 FreeErrCnt_U32;                       /* No of free memory errors */
    UINT32 BlockCnt_aU32[MEM_MAX_NBLOCKSIZES];    /* block counter per different size */
} VOS_MEM_STATISTIC;

typedef struct
{
    VOS_SEM Sem_I32;         /* memeory allocation semaphore */
    BYTE *Area_pU8;         /* Pointer to start of memory area */
    BYTE *FreeArea_pU8;     /* Pointer to start of free part of memory area */
    UINT32 MemSize_U32;      /* Size of memory area */
    UINT32 AllocSize_U32;    /* size of allocated area */
    UINT32 NoOfBlocks_U32;   /* Number of blocks */

    /* free block header array, one entry for each possible free block size */
    struct
    {
        UINT32 Size_U32;            /* block size */
        VOS_MEM_BLOCK *First_pST;      /* pointer to first free block */
    } FreeBlock_aST[MEM_MAX_NBLOCKSIZES];
    VOS_MEM_STATISTIC MemCnt_ST;       /* statistic counters */
} VOS_MEM_CONTROL;

typedef enum
{
    VOS_SEM_EMPTY,
    VOS_SEM_FULL
} VOS_SEM_STATE;

typedef struct
{
    UINT32 QueueAllocated_U32;     //created queue number
    UINT32 QueueMax_U32;           //max created queue number
    UINT32 QueueCreateErrCnt_U32;  //created queue error number
    UINT32 QueueDestroyErrCnt_U32; //Destroyed queue error number
    UINT32 QueueWriteErrCnt_U32;   //queue send error number
    UINT32 QueueReadErrCnt_U32;    //queue read error number
} VOS_QUEUE_CNT;

typedef struct
{
    VOS_QUEUE_CNT QueueCnt_ST;
#ifdef LINUX
    UINT32 QueueNo_U32;
#endif
    VOS_MEM_CONTROL MemCtl_ST; /*the mem ctrl struct*/
    BOOL EcspUsed_B;  /*bool type*/
} VOS_STRUCT;
/******************************************************************************
 *    Global Variable Declare Section
 ******************************************************************************/
extern UINT16 g_vos_debugMask;  /* debug category */

extern VOS_STRUCT g_vos_ST;

#define VOSGLOBAL(x)  g_vos_ST.x

extern void vos_MemDestroy(void);
extern BYTE *vos_MemAlloc(UINT32 size);
extern int vos_MemFree(const BYTE *pAllocated);

#define vos_Destroy      vos_mem_destroy


#define vos_Free         vos_MemFree

/******************************************************************************
 *    Global Prototype Declare Section
 ******************************************************************************/

/* Trace function */
UINT16 vos_GetInfoMask(void);
void vos_SetInfoMask(UINT16 newInfoMask);
UINT16 vos_GetPrintMask(void);
void vos_SetPrintMask(UINT16 newDebugMask);
int vos_GetLogFileSize(void);
void vos_SetLogFileSize(int size);

/* console debug print */
void vos_DPrint(int debugSettingsOverride, char *pFileId, UINT16 lineNr,
                UINT16 category, const char *fmt, int arg1, int arg2,
                int arg3, int arg4, int arg5, int arg6, int arg7);
void vos_DFile(char *pFileName);

/* Semaphores */
int vos_CreateSem(VOS_SEM *pSem, VOS_SEM_STATE initialState);
void vos_DestroySem(VOS_SEM *pSem);
int vos_GetSem(VOS_SEM *pSem, int timeoutInMs);
extern int vos_PutSem(VOS_SEM *pSem);

/* Queues */
int vos_CreateMsgQueue(VOS_QUEUE *pQueueId, int maxNoMsg, int maxLength);
int vos_DestroyMsgQueue(VOS_QUEUE *pQueueId);
int vos_SendMsgQueue(VOS_QUEUE *pQueueId, char *pMsg, UINT32 size);
int vos_ReceiveMsgQueue(VOS_QUEUE *pQueueId, char *pMsg, UINT32 size, int timeout);


/* task and thread */
VOS_THREAD_ID vos_ThreadSpawn(char *pName, int policy, int priority,
                              int stackSize, VOS_THREAD_ROUTINE theFunction, void *pArguments);
int vos_ThreadDelete(VOS_THREAD_ID threadID);

void vos_RegisterCyclicThread(void (*pFunc)(void), char *pName, int interval,
                              int policy, int priority, int stackSize);
void vos_ThreadRun(void);
void vos_ThreadTerminate(void);
void vos_ThreadDone(void);
void vos_TaskDelay(int delayInMs);


UINT32 vos_GetSecTimer(void);
UINT32 vos_GetMilliSecTimer(void);
UINT32 vos_GetMicroSecTimer(void);
UINT32 vos_GetUniqueNumber(void);

void vos_PrintSocketError(void);

extern void vos_QueueShow(void);

/******************************************************************************
 * Name:INT32 vos_DetachSharedMemory( char *SharedMem_pC8)
 * Function: detach the share memory..
 * Input:   SharedMem_pC8,Pointer to shared memory
 * return : ok,if the opearation successed.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern INT32 vos_DetachSharedMemory( char *SharedMem_pC8) ;

/******************************************************************************
 * Name:void vos_Initial(void)
 * Function: init the vos memory buffer.
 * Input:   void
 * return : ok,if the opearation successed.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern void vos_Initial(void);
/******************************************************************************
 *    Multi-Include-Prevent End Section
 ******************************************************************************/
#endif





