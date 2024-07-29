/******************************************************************************
 *File Name :   vos_EdrMessage.h
 *Copyright :   Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date   :   2013/04/01
 *Description   :   generate message function file.
 *
 *REV1.0.0    Moyun   2015/09/11  File Create
*******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef   _VOS_EDR_MESSAGE_H_
#define   _VOS_EDR_MESSAGE_H_


#define MNG_LINUX_VER
/******************************************************************************
*    Include File Section
******************************************************************************/
#ifdef MNG_VXWORKS_VER
#include "msgQLib.h"
#endif
#ifdef MNG_LINUX_VER
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mng_Interface.h"

#endif

/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
#ifdef MNG_VXWORKS_VER

#define VOS_EDR_MSG_Q_ID  MSG_Q_ID   /*the message queue id*/
#define VOS_MSG_NO_WAIT   NO_WAIT    /*no wait variable define*/
#define VOS_MSG_WAIT_FOREVER  WAIT_FOREVER   /*wait for ever*/
#define VOS_MSG_Q_FIFO  MSG_Q_FIFO    /* tasks wait in FIFO order */

#define VOS_MSG_Q_TYPE_MAS MSG_Q_TYPE_MASK	/* mask for pend queue type in options */
#define VOS_MSG_Q_PRIORITY MSG_Q_PRIORITY		/* tasks wait in PRIORITY order */
#define VOS_MSG_Q_EVENTSEND_ERR_NOTIFY MSG_Q_EVENTSEND_ERR_NOTIFY  /* notify when eventRsrcSend fails */
#define VOS_MSG_Q_INTERRUPTIBLE MSG_Q_INTERRUPTIBLE   /* interruptible on RTP signal */
#define IPC_CREAT  (1)
#endif
#ifdef MNG_LINUX_VER
#define VOS_EDR_MSG_Q_ID   key_t
#define VOS_MSG_Q_TYPE_MASK 0x01    /* mask for pend queue type in options */
#define VOS_MSG_Q_FIFO  0x00    /* tasks wait in FIFO order */
#define VOS_MSG_Q_PRIORITY  0x01    /* tasks wait in PRIORITY order */
#define VOS_MSG_Q_EVENTSEND_ERR_NOTIFY 0x02 /* notify when eventRsrcSend fails */
#define VOS_MSG_Q_INTERRUPTIBLE 0x04  /* interruptible on RTP signal */

#define  VOS_MSG_NO_WAIT        (IPC_NOWAIT) /*messge not wait*/
#define  VOS_MSG_WAIT_FOREVER   (0)    /*message wait for ever*/
#endif

/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
/******************************************************************************
* Name:  STATUS vos_EdrMsgQCreate(VOS_EDR_MSG_Q_ID *MsgQId_pI32,INT32 MsgMaxNum_I32,\
                                     INT32 MaxMsgLength_I32,\INT32 MsgOption_I32,int msgflg)
* Function: msg queue create function ,this function should be support the vxworks and the
              linux
* Input:  MsgQId_pI32,msg queue id
*             MsgMaxNum_I32,message queue max size,.
*             MaxMsgLength_I32,message max length for a message.
*             MsgOption_I32,msg queue options, fifo or priority,it is only valid for the vxworks.
* return : OK,if the operation succeesed,otherwise return ERROR.
* Create by moyun ,2015-09-11
*******************************************************************************/
extern STATUS vos_EdrMsgQCreate(VOS_EDR_MSG_Q_ID *MsgQId_pI32,INT32 MsgMaxNum_I32,INT32 MaxMsgLength_I32,INT32 MsgOption_I32,INT32 Msgflg_I32);

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
extern STATUS vos_EdrMsgQSend(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer,UINT32 Bytes_U32, INT32 TimeOut_I32,INT32 Priority_I32);
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
extern STATUS vos_EdrMsgQReceive(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer,UINT32 Bytes_U32, INT32 TimeOut_I32);

/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif

