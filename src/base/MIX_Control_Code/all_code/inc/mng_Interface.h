/******************************************************************************
 *File Name :  mng_Interface.h
 *Copyright :   Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date   :   2013/04/01
 *Description   :  the generate interface function.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
*******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef   _MNG_INTERFACE_H_
#define   _MNG_INTERFACE_H_

/******************************************************************************
*    Include File Section
******************************************************************************/


/******************************************************************************
*    Global Macro Define Section
******************************************************************************/


//#define SERM /*this is the serm device.*/
#define MNG_LINUX_VER
//#define MNG_VXWORKS_VER
//#define MNG_MMC_MODE

#ifdef MNG_VXWORKS_VER
#include "vxworks.h"
#define F_OK  0
#endif

#ifdef MNG_LINUX_VER
#include <stdbool.h>

#ifndef FALSE
#define    FALSE          0
#endif
#ifndef TRUE
#define    TRUE           1
#endif
#ifndef BOOL
typedef  bool           BOOL;
#endif
#ifndef INT8
typedef signed char     INT8;
#endif
#ifndef INT16
typedef short           INT16;
#endif
#ifndef INT32
typedef int             INT32;
#endif
#ifndef UINT8
typedef unsigned char             UINT8;
#endif
#ifndef UINT16
typedef unsigned short            UINT16;
#endif
#ifndef UINT32
typedef unsigned int               UINT32;
#endif
#ifndef CHAR
typedef char                      CHAR;
#endif

#define UINT64  unsigned long long

#define STATUS  int
#define OK      (0)
#define ERROR   (-1)
#ifndef NULL
#define NULL    (0)
#endif
#define WAIT_FOREVER    (-1)
#define taskLock()
#define taskUnlock()
#define PART_ID  int*
typedef void *  (*FUNCPTR)(void *);
#define MSG_PRI_NORMAL   (0)
#define MSG_PRI_URGENT   (0)

#endif

#define MNG_MALLOC_PORT_SIZE_MODE  
/*modify the size from 16 to 32,the mvb is the word mode ,but the user use the data to description the data base the byte,
so the program can support the user config the data base the byte,it should be adjust the size from 16 words to 32 word.
add by moyun 2020-04-18*/
#define MVB_PORT_MAX_SIZE   (32)
#define TRDP_PORT_MAX_SIZE   (1432)
#define PCI_PORT_MAX_SIZE   (1024)
#define MVB_MAX_PORT_SIZE   (4)


/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/

/******************************************************************************
 * Name:  STATUS edr_TaskDelay(INT32 DelayVal_I32)
 * Function: task delay function.
 * Input:  DelayVal_I32,task delay valule
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
extern STATUS vos_EdrTaskDelay(INT32 DelayVal_I32);

/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif






