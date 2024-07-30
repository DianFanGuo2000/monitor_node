/******************************************************************************
 *File Name    : vos_Print.h
 *Copyright    : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  : 2015/03/31
 *Description  : Definitions of debug.
 *
 *REV1.0.0     : TangPH   2015/03/31  File Create
******************************************************************************/

/*******************************************************************************
 *    Multi-Include-Prevent Start Section
 ******************************************************************************/
#ifndef VOS_PRINT_H
#define VOS_PRINT_H

/*******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/*******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "mng_Interface.h"
#include "vos.h"
#include "vos_Time.h"

/*******************************************************************************
 *    Global Macro Define Section
 ******************************************************************************/
#define VOS_DBG_FAULT       (0x4)    /* fault error */
#define VOS_DBG_ERR         (0x3)    /* error */
#define VOS_DBG_WAR         (0x2)    /* warning */
#define VOS_DBG_INFO        (0x1)    /* all */
#define VOS_DBG_MEM         (0x0001)    /* mem*/
/*used to select the information shown ,note prefix INF_ must be 4 chars */
#define VOS_INF_DATETIME   (0x0008)         /* show date and time */
#define VOS_INF_CATEGORY   (0x0004)         /* show category */
#define VOS_INF_LINE       (0x0002)           /* show line number */
#define VOS_INF_FILE       (0x0001)           /* show file name */
#define VOS_INF_ALL        (0xffff)           /* show everything */

#define VOS_DBG_POSITION (char *) (char *)__FILE__, (UINT16)__LINE__

#define VOSPRINT(cat, fmt, p1, p2, p3, p4, p5, p6, p7)  \
do {if(cat >= g_dbg_mask_U16)  \
    {vos_Print(TRUE, VOS_DBG_POSITION, (cat), (fmt), (int)(p1), (int)(p2), \
    (int)(p3), (int)(p4), (int)(p5), (int)(p6), (int)(p7));}} while(FALSE)

#define VOSRAWPRINT(cat, fmt, p1, p2, p3, p4, p5, p6, p7)  \
do {if(cat >= g_dbg_mask_U16)  \
    {vos_Print(FALSE, VOS_DBG_POSITION, (cat), (fmt), (int)(p1), (int)(p2), \
    (int)(p3), (int)(p4), (int)(p5), (int)(p6), (int)(p7));}} while(FALSE)

/* debug printout, waring and error macros */
#define VOS_PRINT0(cat, fmt)                        VOSPRINT(cat, fmt, 0,0,0,0,0,0,0)
#define VOS_PRINT1(cat, fmt, a)                     VOSPRINT(cat, fmt, a,0,0,0,0,0,0)
#define VOS_PRINT2(cat, fmt, a, b)                  VOSPRINT(cat, fmt, a,b,0,0,0,0,0)
#define VOS_PRINT3(cat, fmt, a, b, c)               VOSPRINT(cat, fmt, a,b,c,0,0,0,0)
#define VOS_PRINT4(cat, fmt, a, b, c, d)            VOSPRINT(cat, fmt, a,b,c,d,0,0,0)
#define VOS_PRINT5(cat, fmt, a, b, c, d, e)         VOSPRINT(cat, fmt, a,b,c,d,e,0,0)
#define VOS_PRINT6(cat, fmt, a, b, c, d, e, f)      VOSPRINT(cat, fmt, a,b,c,d,e,f,0)
#define VOS_PRINT7(cat, fmt, a, b, c, d, e, f, g)   VOSPRINT(cat, fmt, a,b,c,d,e,f,g)

#define VOS_RAWPRINT0(cat, fmt)                       VOSRAWPRINT(cat, fmt, 0,0,0,0,0,0,0)
#define VOS_RAWPRINT1(cat, fmt, a)                    VOSRAWPRINT(cat, fmt, a,0,0,0,0,0,0)
#define VOS_RAWPRINT2(cat, fmt, a, b)                 VOSRAWPRINT(cat, fmt, a,b,0,0,0,0,0)
#define VOS_RAWPRINT3(cat, fmt, a, b, c)              VOSRAWPRINT(cat, fmt, a,b,c,0,0,0,0)
#define VOS_RAWPRINT4(cat, fmt, a, b, c, d)           VOSRAWPRINT(cat, fmt, a,b,c,d,0,0,0)
#define VOS_RAWPRINT5(cat, fmt, a, b, c, d, e)        VOSRAWPRINT(cat, fmt, a,b,c,d,e,0,0)
#define VOS_RAWPRINT6(cat, fmt, a, b, c, d, e, f)     VOSRAWPRINT(cat, fmt, a,b,c,d,e,f,0)
#define VOS_RAWPRINT7(cat, fmt, a, b, c, d, e, f, g)  VOSRAWPRINT(cat, fmt, a,b,c,d,e,f,g)

#define VOS_PRINTF   vos_Printf
/*******************************************************************************
 *    Global Structure Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Global Variable Declare Section
 ******************************************************************************/
extern UINT16 g_dbg_mask_U16;

/*******************************************************************************
 *    Global Prototype Declare Section
 ******************************************************************************/
/******************************************************************************
 * Name:void vos_Print
 * Function: base the paramter print the data value
 * Input:   OpenDbgSet_I32,the string print format
        FileName_pC8,the file name
        LineNum_U16,file name
        Cat_U16,category
        Fmt_pC8,format string for print
        Arg1_I32,first of seven required args
        Arg2_I32,second of seven required args
        Arg3_I32,third of seven required args
        Arg4_I32,fourth of seven required args
        Arg5_I32,fiveof seven required args
        Arg6_I32,six of seven required args
        Arg7_I32,seven of seven required args
 * return : void.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern void vos_Print
(
    INT32 OpenDbgSet_I32,char *FileName_pC8,UINT16 LineNum_U16,UINT16 Cat_U16,  \
    const char *Fmt_pC8,INT32 Arg1_I32, INT32 Arg2_I32, INT32 Arg3_I32,INT32 Arg4_I32,\
    INT32 Arg5_I32, INT32 Arg6_I32, INT32 Arg7_I32);

/******************************************************************************
 * Name:void vos_Printf(const char *Fmt_pC8,...)
 * Function: the same with printf function
 * Input:    Fmt_pC8,the string print format
 * return : void.
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern void vos_Printf(const char *Fmt_pC8,...);

/******************************************************************************
 * Name:UINT16 vos_DbgLvlSet(UINT16 Level_U16)
 * Function: set the debug print level
 * Input:    Level_U16,the debug level value
 * return : the debug level value
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern UINT16 vos_DbgLvlSet(UINT16 Level_U16);
/******************************************************************************
* Name:UINT16 vos_DbgLvlGet(void)
* Function: get the debug print level
* Input:    non
* return : the debug level value
* Create by moyun   2014-7-23
*******************************************************************************/
extern UINT16 vos_DbgLvlGet(void);

/******************************************************************************
 * Name:void vos_SysLogInit(void)
 * Function: the syslog init function
 * Input:   non
 * return : non
 * Create by moyun  2014-7-23
 *******************************************************************************/
extern void vos_SysLogInit(void);

/******************************************************************************
 * Name:void vos_SysLog
 * Function: print additional debug information or a raw print
 * Input:    openDbgSet,the debug switch
 *       fName,file name
 *           lNum,the line num
 *           cat ,the category
 *           fmt,format print
 *          arg1...arg8 the debug parameter.
 *
 * return : non
 * Create by moyun  2014-7-23
 *******************************************************************************/

extern void vos_SysLog
(
    INT32 openDbgSet,        /*true if need override debug prINT32*/
    char *fName,           /*file name*/
    UINT16 lNum,           /*line number*/
    UINT16 cat,            /*category*/
    const char *fmt,       /*format string for prINT32*/
    INT32 arg1,              /*first of seven required args*/
    INT32 arg2,
    INT32 arg3,
    INT32 arg4,
    INT32 arg5,
    INT32 arg6,
    INT32 arg7,
    INT32 arg8
);


/*******************************************************************************
 *    Multi-Include-Prevent End Section
 ******************************************************************************/
#endif

