/******************************************************************************
 *File Name    :    vos_EdrMessage.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2013/04/01
 *Description  :    generate message function file.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "mng_Interface.h"
#include "vos_EdrMessage.h"


#define MNG_LINUX_VER


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
 * Name:  STATUS vos_EdrMsgQCreate(VOS_EDR_MSG_Q_ID *MsgQId_pI32,INT32 MsgMaxNum_I32,\
                                         INT32 MaxMsgLength_I32,\INT32 MsgOption_I32,int msgflg)
 * Function: msg queue create function ,this function should be support the vxworks and the
                  linux
 * Input:  MsgQId_pI32,msg queue id
 *            MsgMaxNum_I32,message queue max size,.
 *            MaxMsgLength_I32,message max length for a message.
 *            MsgOption_I32,msg queue options, fifo or priority,it is only valid for the vxworks.
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
STATUS vos_EdrMsgQCreate(VOS_EDR_MSG_Q_ID *MsgQId_pI32,INT32 MsgMaxNum_I32,INT32 MaxMsgLength_I32,INT32 MsgOption_I32,INT32 Msgflg_I32)
{
#ifdef MNG_VXWORKS_VER
    *MsgQId_pI32=NULL; /*the message queue pointer*/
    /*execute the message queue create function*/
    *MsgQId_pI32=msgQCreate(MsgMaxNum_I32,MaxMsgLength_I32, MsgOption_I32);
    /*check the message queue is ok or not*/
    if(*MsgQId_pI32 ==NULL)
    {
        /*return error*/
        return ERROR;
    }
    else
    {
        /*return ok*/
        return OK;
    }
#endif
#ifdef MNG_LINUX_VER
    INT32 RetVal_I32=ERROR; /*define retval variable*/
    key_t KeyVal_ST=*MsgQId_pI32;  /*define key_t variable*/
    RetVal_I32=msgget(KeyVal_ST,Msgflg_I32); /*create the message queue*/
    *MsgQId_pI32=RetVal_I32;  /*write the value to the MsgQId_pI32*/
    return (RetVal_I32);
#endif
}

/******************************************************************************
 * Name:  STATUS vos_EdrMsgQSend(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer,\
                                     UINT32 Bytes_U32, INT32 TimeOut_I32,INT32 Priority_I32)
 * Function: message queue send function,it should be support the vxworks and linux.
 * Input:  MsgQId,message queue id, message queue on which to send
 *            buffer,message to send.
 *            nBytes, bytes num.
 *           timeout,time out value.
             priority,  MSG_PRI_NORMAL or MSG_PRI_URGENT
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
STATUS vos_EdrMsgQSend(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer_pC8,UINT32 Bytes_U32, INT32 TimeOut_I32,INT32 Priority_I32)
{
#ifdef MNG_VXWORKS_VER
    /*if define vxworks version,then execute vxworks mssage queue send fucntion */
    if(msgQSend(MsgQId,Buffer_pC8,Bytes_U32,TimeOut_I32,Priority_I32)==ERROR)
    {
        return ERROR;
    }
    /*check the return value*/
    return (OK);
#endif
#ifdef MNG_LINUX_VER
    INT32 RetVal_I32=0; /*define the Retval_I32 temp variable*/
    /*execute the linux version message queue send function*/
    RetVal_I32=msgsnd(MsgQId,Buffer_pC8,Bytes_U32,TimeOut_I32);
    return RetVal_I32;
#endif
}


/******************************************************************************
 * Name:  STATUS vos_EdrMsgQReceive(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer,UINT32 Bytes_U32, \
                                        INT32 TimeOut_I32)
 * Function: message queue receive function,it should be support the vxworks and linux.
 * Input:  MsgQId,message queue id, message queue on which to send
 *            buffer,message to send.
 *            nBytes, bytes num.
 *           timeout,time out value.
 * return : OK,if the operation succeesed,otherwise return ERROR.
 * Create by moyun ,2015-09-11
 *******************************************************************************/
STATUS vos_EdrMsgQReceive(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer_pC8,UINT32 Bytes_U32, INT32 TimeOut_I32)
{
#ifdef MNG_VXWORKS_VER
    /*execute the vxworks message queue send function*/
    if(msgQReceive(MsgQId,Buffer_pC8,Bytes_U32,TimeOut_I32)==ERROR)
    {
        return ERROR;
    }
    return (OK);
#endif
#ifdef MNG_LINUX_VER
    INT32 RetVal_I32=0;
    /*execute the linux message queue send function*/
    RetVal_I32=msgrcv(MsgQId,(void*)Buffer_pC8,(size_t)Bytes_U32,0,TimeOut_I32);
    return RetVal_I32;
#endif
}


