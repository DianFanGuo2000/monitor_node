/******************************************************************************
 *File Name    :   vos_EdrSemaphore.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2013/04/01
 *Description  :    generate the semaphore function.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "mng_Interface.h"
#include "vos_EdrSemaphore.h"

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
 * Name:INT32 vos_EdrSemInit(VOS_EDR_SEM_ID *Sem_PId,INT32 SemMode_I32,SEM_B_STATE InitValue)
 * Function: init the semaphore,the function is the same as the semBcreate.
 * Input:    Sem_PId,semaphore id;
              SemMode_I32,semphore mode,fifo_mode,or priority mode.
              InitVal, init value, empty or full
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
INT32 vos_EdrSemInit(VOS_EDR_SEM_ID *Sem_PId,INT32 SemMode_I32,enum SEM_B_STATE InitValue)
{
    /*the variable define*/
    INT32 RetVal_I32=0;
#ifdef MNG_LINUX_VER
    /*init the semahore*/
    RetVal_I32= sem_init(Sem_PId,0,InitValue);
    return RetVal_I32;
#endif
#ifdef MNG_VXWORKS_VER
    /*if the system is vxworks,create the bit type semaphore*/
    *Sem_PId=semBCreate(SemMode_I32,(UINT32)InitValue);
    if(NULL==*Sem_PId)
    {
        /*if the semBcreate function is failed,return -1*/
        return -1;
    }
    else
    {
        return 0;
    }
#endif
}


/******************************************************************************
 * Name:INT32  vos_EdrSemPost(VOS_EDR_SEM_ID* Sem_pId)
 * Function: post the semaphore,the function is the same as the semTake.
 * Input:    Sem_PId,semaphore id;
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
INT32  vos_EdrSemPost(VOS_EDR_SEM_ID* Sem_pId)
{
    /*variable define*/
    INT32 RetVal_I32=0;
#ifdef MNG_LINUX_VER
    /*post the semaphore*/
    RetVal_I32= sem_post(Sem_pId);
    return RetVal_I32;
#endif
#ifdef MNG_VXWORKS_VER
    /*semgive the semaphore*/
    RetVal_I32=semGive(*Sem_pId);
    return RetVal_I32;
#endif
}



/******************************************************************************
 * Name:INT32  vos_EdrSemWait(VOS_EDR_SEM_ID* Sem_pId,INT32 Timeout_I32)
 * Function: wait the semaphore,the function is the same as the semTake.
 * Input:    Sem_PId,semaphore id;
              Timeout_I32,time out mode,the parameter is just valid in vxworks mode.
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
INT32 vos_EdrSemWait(VOS_EDR_SEM_ID *Sem_pId,INT32 Timeout_I32)
{
    /*the variable define*/
    INT32 RetVal_I32=0;
#ifdef MNG_LINUX_VER
    /*wait the semahore to come*/
    RetVal_I32=sem_wait(Sem_pId);
    return RetVal_I32;
#endif
#ifdef MNG_VXWORKS_VER
    /*wait the semahore to come*/
    RetVal_I32=semTake(*Sem_pId,Timeout_I32);
    return RetVal_I32;
#endif
}

/******************************************************************************
 * Name: INT32 edr_SemDestory(VOS_EDR_SEM_ID *Sem_pId)
 * Function: destory the semaphore.
 * Input:    Sem_PId,semaphore id;
 * return : -1,if the operation failed,otherwise return 0.
 * Create by moyun  2014-7-23
 *******************************************************************************/
INT32 edr_SemDestory(VOS_EDR_SEM_ID *Sem_pId)
{
    INT32 RetVal_I32=0;
#ifdef MNG_LINUX_VER
    RetVal_I32=sem_destroy(Sem_pId);
#endif
#ifdef MNG_VXWORKS_VER
    RetVal_I32=semDelete(*Sem_pId);
#endif
    return RetVal_I32;
}







