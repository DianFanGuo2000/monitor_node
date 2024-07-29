/******************************************************************************
 *File Name    : vos_define.h
 *Copyright    : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  : 2015/07/10
 *Description  : Definitions of debug.
 *
 *REV1.0.0     : TangPH   2015/07/10  File Create
******************************************************************************/

/*******************************************************************************
 *    Multi-Include-Prevent Start Section
 ******************************************************************************/
#ifndef VOS_DEFINE_H
#define VOS_DEFINE_H
#include "type.h"
/*******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/*******************************************************************************
 *    Include File Section
 ******************************************************************************/

/*******************************************************************************
 *    Global Macro Define Section
 ******************************************************************************/

//#define BPC_MASTER
#define BPC_SLAVER

#define MAX_BRD_NUM             20
#define MAX_BRD_TYPE_LEN        8
#define MAX_BRD_NAME_LEN        16

#define COMMON_CM_CH1         101
#define COMMON_CM_CH2         102
#define COMMON_CM_CH1_LEN     128
#define COMMON_CM_CH2_LEN     32
#define COMMON_CM_CH1_BASE    0
#define COMMON_CM_CH2_BASE    (COMMON_CM_CH1_BASE + COMMON_CM_CH1_LEN)
/*******************************************************************************
 *    Global Structure Define Section
 ******************************************************************************/
typedef enum
{
    RET_ERR = -1,                        /*return error*/
    RET_OK = 0,                          /*return ok*/
    RET_NULL_PTR = 1,                    /*pointer is NULL*/
    RET_OUT_RANGE = 2,                   /*out of range*/
    RET_ILLEGAL_PARA = 3,                /*invalid parameter*/
    RET_TIMEOUT = 4,                     /*time out*/
    RET_READ_REG_FAIL = 5,               /*read register failed*/
    RET_WRITE_REG_FAIL = 6,              /*write register failed*/
    RET_CRC_ERR = 7,                     /*crc error*/
    RET_WRONG_HEAD = 8,                  /*no head flag,wrong file head*/
    RET_MALLOC_ERR = 9,                  /*malloc failed*/
    RET_OPEN_FILE_ERR = 10,              /*open file failed*/
    RET_LENGTH_TOO_LARGE = 11,           /*length too lardge*/
    RET_LENGTH_TOO_SMALL = 12,           /*length too small*/
    RET_TASK_SPAWN_ERR = 13,             /*create task failed*/
    RET_FTP_GETFILE_ERR = 14,            /*ftp get file failed*/
    RET_FILE_HEAD_ERR = 15,              /*file head check failed*/
    RET_WRITE_FILE_ERR = 16,             /*write file failed*/
    RET_FILE_NOT_EXIST = 17,             /*file not exist*/
    RET_DATA_DIFFER = 18,                /*data diffrent*/
    RET_SEND_ERR = 19,                   /*send failed*/
    RET_SEM_CREATE_ERR = 20,             /*sem create error*/
    RET_SEM_DELETE_ERR = 21,             /*sem create error*/
    RET_SEM_GIVE_ERR = 22,               /*sem give error*/
    RET_SEM_TAKE_ERR = 23,               /*sem take error*/
    RET_FILE_DIR_OPEN_ERR = 24,          /*file open error*/
    RET_FILE_DIR_STATUS_ERR = 25,        /*file status get error*/
    RET_SERIAL_TIMEOUT_ERR = 26,         /*serial timeout error*/
    RET_LACK_OF_SPACE = 27,              /*device space not enough to store*/
    RET_SKT_CREATE_ERR = 28,             /*create socket error*/
    RET_SKT_BIND_ERR = 29,               /*bind socket error*/
    RET_SKT_OPTION_SET_ERR = 30,         /*set socket option error*/
    RET_SKT_SEND_ERR = 31,               /*socket send error*/
    RET_SKT_SEND_NOT_ALL = 32,           /*socket send only part of the data*/
    RET_SKT_RECV_ERR = 33,               /*socket receive error*/
    RET_ETH_IF_NOT_EXIST = 34,           /*didn't find wantted ether interface*/
    RET_ELEM_FIND_FIALED = 35,           /*length too lardge*/
    RET_XML_FORMAT_ERR = 36,             /*no head flag,wrong file head*/
    RET_XML_FILE_ERR = 37,               /*malloc failed*/
    RET_XML_LOAD_ERR = 38,               /*open file failed*/
    RET_XML_NODE_TYPE_ERR = 39,             /*create task failed*/
    RET_XML_ADD_NODE_ERR = 40,
    RET_XML_ADD_ATTR_ERR = 41,            /*ftp get file failed*/
    RET_XML_ADD_VALUE_ERR = 42,              /*file head check failed*/
    RET_XML_GET_VALUE_ERR = 43,             /*write file failed*/
    RET_XML_SAVE_FILE_ERR = 44,             /*file not exist*/
    RET_XML_STR_TYPE_ERR = 45,                   /*send failed*/
    RET_XML_FIND_VERSION_ERR = 46,             /*sem create error*/
    RET_ELEMENT_NOT_EXIST = 47,           /**/

    RET_RESERVED                         /*add below*/
} VOS_RET_CODE;


typedef int SOCKET;
typedef unsigned char BYTE;
/*******************************************************************************
 *    Global Variable Declare Section
 ******************************************************************************/

/*******************************************************************************
 *    Global Prototype Declare Section
 ******************************************************************************/

/*******************************************************************************
 *    Multi-Include-Prevent End Section
 ******************************************************************************/
#endif

