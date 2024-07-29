/******************************************************************************
 *File Name :   vos_EdrSemaphore.h
 *Copyright :   Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date   :   2013/04/01
 *Description   :   generate semaphore function file.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
*******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef   _VOS_EDR_SEMAPHORE_H_
#define   _VOS_EDR_SEMAPHORE_H_

#include "mng_Interface.h"

/******************************************************************************
*    Include File Section
******************************************************************************/
#ifdef MNG_LINUX_VER
#include <semaphore.h>
#endif

#define VOS_EDR_SEM_ID   sem_t

/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
#ifdef MNG_VXWORKS_VER
//#define VOS_EDR_SEM_ID   SEM_ID
#endif
#ifdef MNG_LINUX_VER
#define VOS_EDR_SEM_ID   sem_t
#define VOS_SEM_Q_FIFO     0
#define VOS_SEM_Q_PRIORITY 1
//#define SEM_EMPTY      0
//#define SEM_FULL       1

enum  SEM_B_STATE
{
    SEM_EMPTY=0,
    SEM_FULL
};



#endif

/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
/******************************************************************************
 * Name:INT32 vos_EdrSemInit(VOS_EDR_SEM_ID *Sem_PId,INT32 SemMode_I32,SEM_B_STATE InitValue)
 * Function: init the semaphore,the function is the same as the semBcreate.
 * Input:    Sem_PId,semaphore id;
              SemMode_I32,semphore mode,fifo_mode,or priority mode.
              InitVal, init value, empty or full
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern INT32 vos_EdrSemInit(VOS_EDR_SEM_ID *Sem_PId,INT32 SemMode_I32,enum SEM_B_STATE InitValue);

/******************************************************************************
 * Name:INT32  vos_EdrSemPost(VOS_EDR_SEM_ID* Sem_pId)
 * Function: post the semaphore,the function is the same as the semTake.
 * Input:    Sem_PId,semaphore id;
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern INT32  vos_EdrSemPost(VOS_EDR_SEM_ID* Sem_pId);

/******************************************************************************
 * Name:INT32  vos_EdrSemWait(VOS_EDR_SEM_ID* Sem_pId,INT32 Timeout_I32)
 * Function: take the semaphore,the function is the same as the semTake.
 * Input:    Sem_PId,semaphore id;
              Timeout_I32,time out mode,the parameter is just valid in vxworks mode.
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern INT32 vos_EdrSemWait(VOS_EDR_SEM_ID *Sem_pId,INT32 Timeout_I32);


/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif






