/******************************************************************************
 *File Name      :  vos_print.c
 *Copyright      :  Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date    :  2015/07/10
 *Description    :  .
 *
 *REV 1.0.0  TangPH   2015/07/10  File Create
 ******************************************************************************/

/*******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/*******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "mng_Interface.h"
#include "vos_Print.h"
#include <syslog.h>


/*******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/
/*modify by moyun 2016-09-30,modify the g_dbg_mask_U16
value from DGB_INFO  to VOS_DBG_ERR*/
//UINT16 g_dbg_mask_U16 = VOS_DBG_INFO;
UINT16 g_dbg_mask_U16 = VOS_DBG_ERR;

/*******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/
#ifndef LOG_DR680
#if defined (DEBUGMAIN) || defined (DEBUGFSM) || defined (DEBUGSYS) \
        || defined (DEBUGLCP) || defined (DEBUGIPCP) || defined (DEBUGUPAP) \
        || defined (DEBUGCHAP) || defined (DEBUG) || defined (DEBUGIPV6CP)
#define LOG_DR680 LOG_LOCAL2
#else
#define LOG_DR680 LOG_DAEMON
#endif
#endif

/*******************************************************************************
 *    Local Struct  Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/
static char *vos_PrintFNameRmPath(const char* fName);

/*******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/
static FILE *g_dbg_logFile_F = NULL;
//static UINT16 g_dbg_infoMask_U16 = VOS_INF_FILE | VOS_INF_LINE | VOS_INF_DATETIME | VOS_INF_CATEGORY;

/*******************************************************************************
 *    Function Define Section
 ******************************************************************************/

/******************************************************************************
 * Name:static char *vos_PrintFNameRmPath(const char *FileName_pC8 )
 * Function: format the file name string path.
 * Input:    FileName_pC8,the file name string pointer.
 * return : the string pointer.
 * Create by moyun  2014-7-23
 *******************************************************************************/
static char *vos_PrintFNameRmPath(const char *FileName_pC8 )
{
    char *Temp_pC8 = NULL; /*variable define and init*/
    Temp_pC8 = (char *)FileName_pC8 + strlen(FileName_pC8);
    /* find the "\" sign */
    while(('\\' != *Temp_pC8) && ('/' != *Temp_pC8) && (Temp_pC8 >= FileName_pC8))
    {
        Temp_pC8--;
    }
    Temp_pC8++;
    return Temp_pC8;
}


/******************************************************************************
 * Name:void vos_Printf(const char *Fmt_pC8,...)
 * Function: the same with printf function
 * Input:    Fmt_pC8,the string print format
 * return : void.
 * Create by moyun  2014-7-23
 *******************************************************************************/
void vos_Printf(const char *Fmt_pC8,...)
{
    char Buf_aC8[1024]= {0}; /*variable init*/
    va_list VaList_ST; /*variable list struct*/
    va_start(VaList_ST, Fmt_pC8); /*format the variable start*/
    (void)vsprintf(Buf_aC8,Fmt_pC8, VaList_ST); /*format printf the data to the buffer*/
    va_end(VaList_ST);
    (void)puts(Buf_aC8); /*print the string value*/
}

/********************************************************************
Function name  :  vos_print
descripition   :  print additional debug information or a raw print
Parameters     :
return value   :  0 if ok, !0 otherwise
Author         :  TangPH
Create Date    :  2015.07.07
version        :  1.0
modify history :
***********************************************************************/

/******************************************************************************
 * Name:void vos_Print
 * Function: base the paramter print the data value
 * Input:    OpenDbgSet_I32,the string print format
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
void vos_Print
(
    INT32 OpenDbgSet_I32,char *FileName_pC8,UINT16 LineNum_U16,UINT16 Cat_U16,\
    const char *Fmt_pC8,INT32 Arg1_I32, INT32 Arg2_I32, INT32 Arg3_I32,INT32 Arg4_I32,\
    INT32 Arg5_I32, INT32 Arg6_I32, INT32 Arg7_I32)
{
    char DateTime_aC8[50]= {0}; /*variable define*/
    /*if openDbgSet set true, will print the additional message*/
    if(OpenDbgSet_I32)
    {
        /*check the print type*/
        switch(Cat_U16)
        {
            /*fault*/
            case VOS_DBG_FAULT:
                VOS_PRINTF("FAULT: ");
                break;
            /*error*/
            case VOS_DBG_ERR:
                VOS_PRINTF("ERROR: ");
                break;
            /*waring*/
            case VOS_DBG_WAR:
                VOS_PRINTF("WARNING: ");
                break;
            default:
                break;
        }
        /*check print the data or not*/
        if(Cat_U16>= VOS_DBG_WAR)
        {
            /*get the date time*/
            vos_DateStringGet(DateTime_aC8, sizeof(DateTime_aC8));
            VOS_PRINTF("%s ",DateTime_aC8);
            /*get the file name*/
            VOS_PRINTF("File Name: %s ", vos_PrintFNameRmPath(FileName_pC8));
            VOS_PRINTF("LINE NUM: %d ", (int)LineNum_U16);
        }
    }
    /*if openDbgSet set FALSE, call the raw printf*/
    VOS_PRINTF(Fmt_pC8, Arg1_I32, Arg2_I32, Arg3_I32, Arg4_I32, Arg5_I32, Arg6_I32, Arg7_I32);
}


/******************************************************************************
 * Name:UINT16 vos_DbgLvlSet(UINT16 Level_U16)
 * Function: set the debug print level
 * Input:    Level_U16,the debug level value
 * return : the debug level value
 * Create by moyun  2014-7-23
 *******************************************************************************/
UINT16 vos_DbgLvlSet(UINT16 Level_U16)
{
    g_dbg_mask_U16 = Level_U16;
    return g_dbg_mask_U16;
}
/******************************************************************************
 * Name:UINT16 vos_DbgLvlGet(void)
 * Function: get the debug print level
 * Input:   non
 * return : the debug level value
 * Create by moyun  2014-7-23
 *******************************************************************************/
UINT16 vos_DbgLvlGet(void)
{
    VOS_PRINT1(VOS_DBG_INFO, "VOS DBG LEVEL:%d\n", g_dbg_mask_U16);
    return g_dbg_mask_U16;
}


/******************************************************************************
 * Name:void vos_SysLogInit(void)
 * Function: the syslog init function
 * Input:   non
 * return : non
 * Create by moyun  2014-7-23
 *******************************************************************************/

void vos_SysLogInit(void)
{
    openlog("DR680", LOG_PID | LOG_NDELAY, LOG_DR680);
    setlogmask(LOG_UPTO(LOG_INFO));
}


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

void vos_SysLog
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
)
{
    /*if openDbgSet set true, will prINT32 the additional message*/
    if(openDbgSet)
    {
        switch(cat)
        {
            case VOS_DBG_FAULT:
                syslog(LOG_CRIT, "FAULT:\nFile Name: %s\nLINE NUM: %d",
                       vos_PrintFNameRmPath(fName), (INT32)lNum);
                syslog(LOG_CRIT, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                break;
            case VOS_DBG_ERR:
                syslog(LOG_ERR, "ERROR:\nFile Name: %s\nLINE NUM: %d",
                       vos_PrintFNameRmPath(fName), (INT32)lNum);
                syslog(LOG_ERR, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                break;
            case VOS_DBG_WAR:
                syslog(LOG_ERR, "WARNING:\nFile Name: %s\nLINE NUM: %d",
                       vos_PrintFNameRmPath(fName), (INT32)lNum);
                syslog(LOG_ERR, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                break;
            case VOS_DBG_INFO:
                syslog(LOG_INFO, "%s,LINE: %d",vos_PrintFNameRmPath(fName), (INT32)lNum);
                syslog(LOG_INFO, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                break;
            default:
                break;
        }
    }
    /*if openDbgSet set FALSE, call the raw printf*/
}


