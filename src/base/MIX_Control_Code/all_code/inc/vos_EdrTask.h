/******************************************************************************
 *File Name :   vos_EdrTask.h
 *Copyright :   Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date   :   2013/04/01
 *Description   :   generate task create functiron file.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
*******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef   _VOS_EDR_TASK_H_
#define   _VOS_EDR_TASK_H_

/******************************************************************************
*    Include File Section
******************************************************************************/
#ifdef MNG_VXWORKS_VER
#include "tasklib.h"

#endif
#ifdef MNG_LINUX_VER
#include <pthread.h>

#endif
#include "mng_Interface.h"

/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
#ifdef MNG_VXWORKS_VER
#define VOS_TASK_BASE_PRIORITY   (70) /*the vxworks system adjust task priority value*/
#define VOS_VX_FP_TASK  VX_FP_TASK
#endif
#ifdef MNG_LINUX_VER
#define VOS_TASK_BASE_PRIORITY   (99)/*the linux system adjust task priority value*/

/*define the VOS_VX_FP_TASK value to 0,in order to compability linux */
#define VOS_VX_FP_TASK    (0)
#endif

/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
/******************************************************************************
 * Name:  STATUS edr_TaskSpawn(const char *TaskName_pc8,INT32 Priority_I32,\
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
extern INT32 vos_EdrTaskSpawn(const char *TaskName_pc8,INT32 Priority_I32,INT32 Policy_I32, INT32 StackSize_I32, FUNCPTR exec_fun,INT32 Par_I32);
/******************************************************************************
 * Name:void SetThreadName(char *TaskName_pC8)
 * Function: set the thread name.
 * Input:    TaskName_pC8,the task name;
 * return : non
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern void SetThreadName(char *TaskName_pC8);

/******************************************************************************
 * Name:  BOOL vos_IsEdrTaskExist( INT32 ThreadId_I32)
 * Function:check the thread exist or not.
 * Input:
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/

extern BOOL vos_IsEdrTaskExist( INT32 ThreadId_I32);


/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif






