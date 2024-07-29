/******************************************************************************
 *File Name    :    vos_cmd.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2015/10/10
 *Description  :    memory manage
 *
 *
 *REV1.0.0    WangDJ   2015/10/10  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "vos_cmd.h"

/******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/

/******************************************************************************
 *    Global Variable Define Section
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
#ifdef LINUX

/****************************************************
 *Name        :
 *Function     : .
 *para         :   void
 *return       : void
 *Create by author   year/month/date
****************************************************/
int vos_system(const char *cmd, INT32 *cmdRet)
{
    INT32 shellTerminationStatus=0;/**/
    INT32 ret=0;/**/
    if (0==cmd)
    {
        printf("%s:invalid cmd\n", __func__);
        return -1;
    }
    shellTerminationStatus=system(cmd);
    if ((-1==shellTerminationStatus )|| (127==shellTerminationStatus) )
    {
        printf("%s:error, shellTerminationStatus=%d:%s\n",
               __func__, shellTerminationStatus, strerror(errno));
        return -1;
    }
    if (0==cmdRet)
    {
        return 0;
    }
    if (WIFEXITED(shellTerminationStatus))
    {
        ret=WEXITSTATUS(shellTerminationStatus);
    }
    else if (WIFSIGNALED(shellTerminationStatus))
    {
        ret=WTERMSIG(shellTerminationStatus);
        printf("%s:shell abnormal termination, signal number=%d%s\n",
               __func__, WTERMSIG(shellTerminationStatus),
#ifdef WCOREDUMP
               WCOREDUMP(shellTerminationStatus)?"(core file generated)":"");
#else
               "");
#endif
    }
    else if (WIFSTOPPED(shellTerminationStatus))
    {
        ret=WSTOPSIG(shellTerminationStatus);
        printf("%s:shell stopped, signal number=%d\n", __func__,
               WSTOPSIG(shellTerminationStatus));
    }
    *cmdRet=ret;
    return 0;
}
#endif
