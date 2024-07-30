/******************************************************************************
 *File Name    :    vos_mem.h
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2015/05/06
 *Description  :    vos memory head file
 *
 *REV1.0.0    zzr   2010/01/26  File Create
******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef VOS_MEM_H
#define VOS_MEM_H

/******************************************************************************
*    Debug switch Section
******************************************************************************/

/******************************************************************************
*    Include File Section
******************************************************************************/
#include "mng_Interface.h"
#include "vos.h"
/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
#define G_VOS_MEM_CTL g_vos_ST.MemCtl_ST
/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/

/******************************************************************************
*    Global Variable Declare Section
******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
/******************************************************************************
 * Name: vos_MemInit
 * Function:    Initialize memory area.
 *           After init memory blocks are pre-allocated and freed.
*           To avoid problem with too many small blocks and no large.
*           memory should have been allocated and set up in the global memory
*           control block.
 * Input:  MemSize_U32,the memory size value
 * return :0 if OK,!=0 if error
 * Create by zzr  2015-05-05
 *******************************************************************************/
extern INT32 vos_MemInit(UINT32 MemSize_U32);
/******************************************************************************
 * Name: void vos_MemDestroy(void)
 * Function:   shutdown memory are
 * Input:  non
 * return :non
 * Create by zzr  2015-05-05
 *******************************************************************************/
extern void vos_MemDestroy(void);
/******************************************************************************
* Name: vos_MemAlloc
* Function:   allocate memory and return allocated buffer size
* Input: Size_U32, size of wanted memory area
* return :pointer to memory area, of NULL if error.
* Create by zzr  2015-05-05
*******************************************************************************/
extern BYTE *vos_MemAlloc(UINT32 Size_U32);
/******************************************************************************
* Name: INT32 vos_MemFree(const BYTE *Allocated_pU8)
* Function:  free allocated memory  the freed memory block is added to a free
*            block linked list, for future use.
* Input:   Allocated_pU8,pointer to memory block to free
* return :   0 if OK, !=0 if not
* Create by zzr  2015-05-05
*******************************************************************************/
extern INT32 vos_MemFree(const BYTE *Allocated_pU8);
/******************************************************************************
* Name: vos_MemAllocCntGet
* Function:  get the current value of allocated buffers.
* Input:
* return :  current value of allocated buffers
* Create by zzr  2015-05-05
*******************************************************************************/
extern UINT32 vos_MemAllocCntGet(void);
/******************************************************************************
* Name: vos_MemShow
* Function:  displays the momory usage and statistic
* Input:     non
* return :  non
* Create by zzr  2015-05-05
*******************************************************************************/
extern void vos_MemShow(void);

/******************************************************************************
* Name: vos_MemBelongVos
* Function:   checks if a pointer is a pointer into the TRDP vos memory area.
* Input:    Data_pU8,the memory pointer
* return :  TRUE if it is, FALSE if it isn't
* Create by zzr  2015-05-05
*******************************************************************************/
extern INT32 vos_MemBelongVos(const BYTE *Data_pU8);

/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif







