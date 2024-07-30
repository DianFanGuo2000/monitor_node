/******************************************************************************

 *File Name    : vos_com.h

 *Copyright    : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.

 *Create Date  : 2012/3/14

 *Description  :

 *REV1.0.0     : lisy   2012/3/14  File Create


******************************************************************************/

/******************************************************************************
 *    Multi-Include-Prevent Start Section
 ******************************************************************************/
#ifndef VOSCOM_H
#define VOSCOM_H

/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/


/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include <stdio.h>

#if defined(VXWORKS)
#include <vxWorks.h>
#endif

/******************************************************************************
 *    Global Macro Define Section
 ******************************************************************************/
/* VOS version */
#define VOS_VERSION         (1)
#define VOS_RELEASE         (0)
#define VOS_UPDATE          (0)
#define VOS_EVOLUTION       (1)

/* VOS status information returned from VOS_getStatus */
#define VOS_RUN                    (0)
#define VOS_IAPC_NOT_CONFIGURED    (1)
#define VOS_NOT_INITIATED          (2)

/* VOSCom event selectors for VOS_handleEvent */
#define VOS_EVENT_NONE     (0)   /* pArg is ignored */
#define VOS_EVENT_LINK_UP  (1)   /* pArg is ignored */

/* Time-out values */
#define VOS_NO_WAIT          (0)
#define VOS_WAIT_FOREVER    (-1)

/* Default memory pool size:5MB */
#define VOS_MEMORY_DEFAULT (0x500000)

/* maximum size of file name and path */
#define VOS_SIZEOF_FILEPATH   (255)

#define MAX_FORMAT_LEVEL      (5)  /* max levels in dataset formattVOSfor recursion) */

/* datatypes for id in configuration dataset formatting table */
#define VOS_BOOLEAN1        (-1)    /* kept for backward compatibility, replaced by VOS_BOOLEAN8 */
#define VOS_BOOLEAN8        (-1)
#define VOS_CHAR8           (-2)
#define VOS_UNICODE16       (-3)
#define VOS_INT8            (-4)
#define VOS_INT16           (-5)
#define VOS_INT32           (-6)
#define VOS_UINT8           (-7)
#define VOS_UINT16          (-8)
#define VOS_UINT32          (-9)
#define VOS_REAL32         (-10)
#define VOS_STRING         (-11)
#define VOS_ARRAY          (-12)
#define VOS_RECORD         (-13)
#define VOS_INT64          (-14)
#define VOS_UINT64         (-15)
#define VOS_TIMEDATE48     (-16)
#define VOS_MAX_TYPE       (-16)


#define VOS_VAR_SIZE           (0)  /* size to indicate variable size */


/* default com-parameter-id */
#define VOS_DEF_COMPAR_PD_ID    (1)   /* default com-parameter-id for PD */
#define VOS_DEF_COMPAR_MD_ID    (2)   /* default com-parameter-id for MD */
#if !IAPC_IN_USE
#define VOS_DEF_COMPAR_ETBN_PD_ID  (101) /* default com-parameter-id for ETBN PD communication*/
#define VOS_DEF_COMPAR_ETBN_MD_ID  (102) /* default com-parameter-id for ETBN MD communication*/
#endif
#define VOS_INVALID_ZERO         (0)     /* Zero value if invalid */
#define VOS_INVALID_KEEP         (1)     /* Keep value if invalid */

/* Error code: type definitions */
#define TYPE_EXCEPTION   (0xc0000000)
#define TYPE_ERROR       (0x80000000)
#define TYPE_WARNING     (0x40000000)

/* Error code: VOS function definitions */
#define ERR_VOSCOM    (0x00030000)
#define ERR_IAPCCOM      (0x00040000)

/* Error code: global/local definitions */
#define ERR_LOCAL       (0x00008000)


/* Error codes */
#define VOS_OK             (0)     /* General OK value */
#define VOS_ERROR         (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x65)  /* General error value */
#define VOS_NOT_FOUND     (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x66)  /* Item was not found */
#define VOS_ILLEGAL_SIZE   (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x67)  /* illegal size */
#define VOS_FCS_ERROR      (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x68)  /* frame check sequence error */
#define VOS_MEM_ERROR    (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x69)   /* Out of memory */
#define VOS_PARSE_ERROR   (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x6A)  /* Error during parsing of XML file */
#define VOS_MARSHALL_ERROR      (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x6B)  /* Error during marshalling */
#define VOS_MARSHALL_MAX_LEVEL   (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x6C) /* reached max level of recursion during marshlling */
#define VOS_MARSHALL_UNKNOWN_DATASETID  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x6D) /* dataset ID unknown */
#define VOS_MARSHALL_TOO_BIG             (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x6E)  /* marshall too big */
#define VOS_ENDIAN_ALIGNMENT_ERROR  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x6F)   /* CPU does
                                         not work as expected concerning alignment or endian */
#define VOS_TAB_ERR_INIT   (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x70)    /* table not initialized */
#define VOS_TAB_ERR_NOT_FOUND  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x71) /* item was not found */
#define VOS_TAB_ERR_ILLEGAL_SIZE (TYPE_ERROR | ERR_VOSCOM | ERR_LOCAL | 0x72)  /* illegal size */
#define VOS_TAB_ERR_EXISTS       (TYPE_ERROR | ERR_VOSCOM | ERR_LOCAL | 0x73)       /* item already exists with different parameters */
#define VOS_SEM_ERROR          (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x74)  /* Semaphore error */
#define VOS_MEM_ILL_RETURN         (TYPE_ERROR | ERR_VOSCOM | ERR_LOCAL | 0x75)  /* return of memory not previously allocated */
#define VOS_QUEUE_ERROR         (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x76)  /* Queue error */
#define VOS_QUEUE_IN_USE      (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x77) /* attempt to destory a queue in use */
#if IAPC_IN_USE
#define VOS_IAPC_NOT_READY      (TYPE_WARNING | ERR_VOSCOM | ERR_LOCAL | 0x78)  /* IAPC is waiting for
                                                                            configuration from VOSDir */
#endif
#define VOS_ERR_NO_IPADDR          (TYPE_ERROR | ERR_VOSCOM | ERR_LOCAL | 0x79)  /* Own IP address not found */
#define VOS_ERR_NO_ETH_IF          (TYPE_WARNING | ERR_VOSCOM | ERR_LOCAL | 0x80)  /* Ethernet interface not initiated */

#define VOS_INVALID_PAR    (TYPE_WARNING | ERR_VOSCOM | ERR_LOCAL | 0xC9)     /* invalid parameters */
#define VOS_INVALID_COMPAR   (TYPE_WARNING | ERR_VOSCOM | ERR_LOCAL | 0xCA)  /* invalid communication parameters */
#define VOS_INVALID_DATA      (TYPE_ERROR| ERR_VOSCOM | ERR_LOCAL | 0xCB)
#define VOS_MD_NOT_INIT       (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0xCC)  /* MD communication not initiated */
#define VOS_WRONG_TOPO_CNT    (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0xCD) /* current topo counter not equal to application topo counter */

/* net driver error codes */
#define VOS_ERR_INIT_DRIVER_FAILED      (0x12D)
#define VOS_ERR_EXIT_DRIVER_FAILED     (0x12E)
#define VOS_ERR_DRIVER_ALREADY_STARTED (TYPE_WARNING | ERR_VOSCOM | ERR_LOCAL | 0x12F) /* driver is already open */
#define VOS_ERR_DRIVER_ALREADY_CLOSED (TYPE_WARNING | ERR_VOSCOM | ERR_LOCAL | 0x130) /* driver is already closed */
#define VOS_ERR_MD_SOCKET_CREATE_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x131) /* could not create MD socket */
#define VOS_ERR_MD_SOCKET_BIND_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x132)   /* could not bind MD socket */
#define VOS_ERR_PD_SOCKET_CREATE_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x133) /* could not create PD socket */
#define VOS_ERR_PD_SOCKET_BIND_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x134)   /* could not bind PD socket */
#define VOS_ERR_SEM_CREATE_FAILED        0x135
#define VOS_ERR_PD_SOCKOPT_FAILED (TYPE_ERROR | ERR_VOSCOM | ERR_LOCAL | 0x136)  /* setsockopt for PD socket failed */
#define VOS_ERR_MD_SOCKOPT_FAILED (TYPE_ERROR | ERR_VOSCOM | ERR_LOCAL | 0x137)  /* setsockopt for MD socket failed */
#define VOS_ERR_MD_SENDTO_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x138) /* sendto on MD socket failed */
#define VOS_ERR_SNMP_SENDTO_FAILED (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x139) /* send on SNMP socket failed */
#define VOS_ERR_PD_SENDTO_FAILED (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x140) /* send on PD socket failed */
#define VOS_ERR_SNMP_SOCKET_CREATE_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x141) /* could not create SNMP socket */
#define VOS_ERR_SNMP_SOCKET_BIND_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x142)   /* could not bind SNMP socket */

#define VOS_ERR_MD_SOCKET_LISTEN_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x143)   /* could not listen MD socket */
#define VOS_ERR_MD_SOCKET_ACCEPT_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x144)   /* could not accpet MD socket */
#define VOS_ERR_MD_SOCKET_CONNECT_FAILED  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x145)  /* could not connect MD socket */
#define VOS_ERR_MD_SEND_FAILED (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x146) /* MD socket send failed */

/* simulation error codes */
#define VOS_SIM_ERR_PCAP_IFOPEN    (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x300) /* Pcap could not access/open interface */

/* my own Error codes added in VOS : lisy added */
#define VOS_GET_HOST_IP_ERROR (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x200)  /* get own IP address error */
#define VOS_OWN_IP_NOT_FOUND   (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x201) /* could not found own IP address */
#if IAPC_IN_USE
#define VOS_MULT_IF_SET_ERROR  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x202) /* set local multicast IF error */
#else
#define VOS_MULT_LACAL_IF_SET_ERROR  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x202) /* set local multicast IF error */
#define VOS_MULT_ETBN_IF_SET_ERROR  (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0x203) /* set ETBN multicast IF error */
#endif
#define VOS_LONG_PACKET_ERROR (TYPE_EXCEPTION | ERR_VOSCOM | ERR_LOCAL | 0X204) /* MD send long packets error */

/* XML parser declarations */
#define MAX_SRC_FILTER 8  /* max number of PD receive filter per comid */
#define MAX_URI_LEN   VOS_MAX_URI_LEN   /* MAX length of URI string */
#define MAX_TOKLEN (MAX_SRC_FILTER * MAX_URI_LEN)  /* MAX length of token string */
#define MAX_TAGLEN 132    /* max length of tag length */



/******************************************************************************
 *    Global Structure Define Section
 ******************************************************************************/

/******************************************************************************
 *    Global Variable Declare Section
 ******************************************************************************/

/******************************************************************************
 *    Global Prototype Declare Section
 ******************************************************************************/

/******************************************************************************
 *    Multi-Include-Prevent End Section
 ******************************************************************************/
#endif

