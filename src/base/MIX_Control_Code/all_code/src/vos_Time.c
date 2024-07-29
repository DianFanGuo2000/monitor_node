/******************************************************************************
 *File Name      :  vos_Time.c
 *Copyright      :  Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date    :  2015/07/13
 *Description    :  the vos time interface function
 *
 *REV 1.0.0  moyun   2018/03/20  File Create
 ******************************************************************************/

/*******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/*******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "vos_Define.h"
#include "vos_Time.h"


/*******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Local Struct  Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/

/*******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Function Define Section
 ******************************************************************************/


/******************************************************************************
 * Name:  void vos_DateStringGet(char *pString, UINT32 bufLen)
 * Function: get the vos date string.
 * Input:  String_pC8,the time string
              BufLen_U32,the buffer length
 * return : non.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
void vos_DateStringGet(char *String_pC8, UINT32 BufLen_U32)
{
    struct timespec CurTime_ST;
    struct tm LocTime_ST;
    char ErrBuf_aC8[80]= {0};
    /* get the current time of the clock
     * CLOCK_REALTIME: system wide realtime clock, time-of-day time */
    if(RET_ERR == clock_gettime(CLOCK_REALTIME, &CurTime_ST))
    {
        (void)snprintf(String_pC8, BufLen_U32, " ");
        (void)strerror_r(errno, ErrBuf_aC8, 80);
        VOS_PRINT2(VOS_DBG_ERR, "vos_DateStringGet failed: clock_gettime VxWorks errno=%#x %s\n",
                   errno, ErrBuf_aC8);
    }
    else
    {
        /* convert to local time , localtime_r convert the calendar time into
         * broken-down time, expressed as local time */
        (void)localtime_r(&CurTime_ST.tv_sec, &LocTime_ST);
        (void)snprintf(String_pC8, BufLen_U32, "%04d-%02d-%02d %02d:%02d:%02d",
                       1900+(LocTime_ST.tm_year), 1+(LocTime_ST.tm_mon), LocTime_ST.tm_mday,
                       LocTime_ST.tm_hour, LocTime_ST.tm_min, LocTime_ST.tm_sec);
        (void)snprintf(String_pC8+19, BufLen_U32-19, ":%ld:%ld",
                       CurTime_ST.tv_sec,CurTime_ST.tv_nsec);
    }
}

