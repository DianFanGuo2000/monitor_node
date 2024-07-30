/******************************************************************************
 *File Name    :    type.h
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2015/08/24
 *Description  :    .
 *
 *REV1.0.0   wangdj  2015-08-24  File Create
 *
******************************************************************************/
/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/

/******************************************************************************
*    Debug switch Section
******************************************************************************/

/******************************************************************************
*    Include File Section
******************************************************************************/
#ifndef TYPE_H_HHHH
#define TYPE_H_HHHH
#include <stdio.h>
#include <stdlib.h>

#ifdef LINUX
#include <stdbool.h>
#endif
/******************************************************************************
*    Global Macro Define Section
******************************************************************************/

/* vxWorks types */
#ifdef VXWORKS

#ifndef CHAR
typedef char            CHAR;
#endif

#ifndef VOID
typedef VOID                      void;
#endif

/* Linux types */
#elif defined LINUX

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
//typedef unsigned long               UINT32;
#endif
//typedef unsigned long             UINT64;

typedef char                      CHAR;


#endif
#endif
/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/

