/******************************************************************************
 *File Name    :    vos_TaskDelay.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2013/04/01
 *Description  :    vos task delay function file.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "vos_EdrMessage.h"
#ifdef MNG_LINUX_VER
#include <unistd.h>
#include <time.h>
#endif

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
 * Name:  STATUS vos_EdrTaskDelay(INT32 DelayVal_I32)
 * Function: task delay function.
 * Input:  DelayVal_I32,task delay valule
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
STATUS vos_EdrTaskDelay(INT32 DelayVal_I32)
{
#ifdef MNG_LINUX_VER
    STATUS RetVal_I32=ERROR;
    /*the struct variable DelayTime_ST define*/
    struct timespec DelayTime_ST;
    /*the struct variableRemainTime_ST define*/
    struct timespec RemainTime_ST;
    /*check the dealy value*/
    if (DelayVal_I32 >= 1000 )
    {
        /*the data value is ms,change to sec need to div 1000*/
        DelayTime_ST.tv_sec = DelayVal_I32 / 1000;
        /*set the delay time nsec*/
        DelayTime_ST.tv_nsec = (DelayVal_I32 % 1000) * 1000000;
    }
    else
    {
        /*set the delay time sec*/
        DelayTime_ST.tv_sec = 0;
        /*set the delay time nsec*/
        DelayTime_ST.tv_nsec = DelayVal_I32 * 1000000;
    }
    /*execute the delay function*/
    RetVal_I32 = nanosleep(&DelayTime_ST, &RemainTime_ST);
    /*check the delay time is ok or not*/
    while ((RetVal_I32 != 0) )
    {
        /* The sleep was interrupted,
          Wait the remaining time */
        RetVal_I32= nanosleep(&RemainTime_ST, &RemainTime_ST);
    }
    return RetVal_I32;
#endif
    
#ifdef MNG_VXWORKS_VER
   return  taskDelay(DelayVal_I32);
#endif
   
}



