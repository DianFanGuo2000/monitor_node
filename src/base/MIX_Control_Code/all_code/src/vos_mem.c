/******************************************************************************
 *File Name    :    vos_mem.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2015/05/06
 *Description  :    memory manage
 *
 *
 *REV1.0.0    zzr   2015/05/06  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "mng_Interface.h"
#include <stdio.h>
#include <stdlib.h>
#include "vos_Print.h"
#include "vos_Define.h"
#include "vos_mem.h"

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
static BYTE *vos_MemAllocBuf(UINT32 Size_U32, UINT32 *BufSize_pU32);

/******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/

/******************************************************************************
 *    Function Define Section
 ******************************************************************************/
/******************************************************************************
* Name: vos_MemAllocBuf
* Function:   allocate memory and return allocated buffer size
* Input:  size, size of wanted memory area
*         pBufSize, pointer to buffer size
* return :pointer to memory area, of NULL if error.
* Create by zzr  2015-05-05
*******************************************************************************/
static BYTE *vos_MemAllocBuf(UINT32 Size_U32, UINT32 *BufSize_pU32)
{
    UINT32 i=0, BlockSize_U32=0U;
    VOS_MEM_BLOCK *Block_pST = NULL;
    if(NULL ==BufSize_pU32)
    {
        return (BYTE *)NULL;
    }
    /* adjust size to get one which is a multiple of UINT32's */
    Size_U32 = ((Size_U32 + sizeof(UINT32) - 1) / sizeof(UINT32)) * sizeof(UINT32);
    /* find appropriate blockSize */
    for(i = 0; i < G_VOS_MEM_CTL.NoOfBlocks_U32; i++)
    {
        /* trdp_vosMemInit has init the freeBlock, it assign value to freeBlock.size */
        if(Size_U32 <= G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32)
        {
            break;
        }
    }
    if(i >= G_VOS_MEM_CTL.NoOfBlocks_U32)
    {
        G_VOS_MEM_CTL.MemCnt_ST.AllocErrCnt_U32++;
        VOS_PRINT1(VOS_DBG_ERR, " vos_MemAllocBuf No block size big enough, requested size=%d\n", Size_U32);
        *BufSize_pU32 = 0;
        return (BYTE *)NULL;  /* no block size big enough */
    }
    /* get memory semaphore */
    if(RET_OK != vos_GetSem(&G_VOS_MEM_CTL.Sem_I32, VOS_SEM_WAIT_FOREVER))
    {
        G_VOS_MEM_CTL.MemCnt_ST.AllocErrCnt_U32++;
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemAllocBuf can't get semaphore\n");
        *BufSize_pU32 = 0;
        return (BYTE *)NULL;
    }
    BlockSize_U32 = G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32;
    Block_pST = G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST;
    /* check if there is a free block ready */
    if(NULL != Block_pST)
    {
        /* there is, get it. set start pointer to next free block in the linked list
         * if there are blocks have been freed, point to the latest one freed, and return the pointer.
         * then forward the pFirst point to the second latest one freed */
        G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST = Block_pST->Next_pST;
    }
    else
    {
        /* there was no suitable free block, create one from the free area */
        /* enough free memory left? */
        if((G_VOS_MEM_CTL.AllocSize_U32 + BlockSize_U32 + sizeof(VOS_MEM_BLOCK)) < G_VOS_MEM_CTL.MemSize_U32)
        {
            Block_pST = (VOS_MEM_BLOCK *)G_VOS_MEM_CTL.FreeArea_pU8; /* lint !e826 allocation of VOS_MEM_BLOCK from free area */
            G_VOS_MEM_CTL.FreeArea_pU8 = (BYTE *)G_VOS_MEM_CTL.FreeArea_pU8 + (sizeof(VOS_MEM_BLOCK) + BlockSize_U32);
            G_VOS_MEM_CTL.AllocSize_U32 += BlockSize_U32 + sizeof(VOS_MEM_BLOCK);
            G_VOS_MEM_CTL.MemCnt_ST.BlockCnt_aU32[i]++;
        }
        else
        {
            while((++i < G_VOS_MEM_CTL.NoOfBlocks_U32) && (NULL == Block_pST))
            {
                Block_pST = G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST;
                if(NULL != Block_pST)
                {
                    VOS_PRINT2(VOS_DBG_ERR, "vos_MemAllocBuf used a bigger buffer size=%d asked size=%d\n",
                               G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32, Size_U32);
                    /* there is, get it. set start pointer to next free block in the linked list */
                    G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST = Block_pST->Next_pST;
                    BlockSize_U32 = G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32;
                }
            }
            if(NULL == Block_pST)
            {
                /* not enough memory */
                VOS_PRINT0(VOS_DBG_ERR, "vos_MemAlloc not enough memory\n");
            }
        }
    }
    /* release semaphore */
    if(RET_OK != vos_PutSem(&G_VOS_MEM_CTL.Sem_I32))
    {
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemAllocBuf give semaphore ERROR\n");
    }
    if(NULL != Block_pST)
    {
        Block_pST->Size_U32 =BlockSize_U32;
        G_VOS_MEM_CTL.MemCnt_ST.FreeSize_U32 -= BlockSize_U32 + sizeof(VOS_MEM_BLOCK);
        if(G_VOS_MEM_CTL.MemCnt_ST.FreeSize_U32 < G_VOS_MEM_CTL.MemCnt_ST.MinFreeSize_U32)
        {
            G_VOS_MEM_CTL.MemCnt_ST.MinFreeSize_U32 =G_VOS_MEM_CTL.MemCnt_ST.FreeSize_U32;
        }
        G_VOS_MEM_CTL.MemCnt_ST.AllocCnt_U32++;
        /* return pointer to data area, not the memory block itself */
        *BufSize_pU32=BlockSize_U32;
        return (BYTE *)Block_pST + sizeof(VOS_MEM_BLOCK);
    }
    else
    {
        G_VOS_MEM_CTL.MemCnt_ST.AllocErrCnt_U32++;
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemAllocBuf malloc memory ERROR\n");
        *BufSize_pU32 = 0;
        return (BYTE *)NULL;
    }
}

/******************************************************************************
 * Name: vos_mem_init
 * Function:    Initialize memory area.
 *                        After init memory blocks are pre-allocated and freed.
 *              To avoid problem with too many small blocks and no large.
 *              memory should have been allocated and set up in the global memory
 *              control block.
 * Input:  MemSize_U32,the memory size value
 * return :0 if OK,!=0 if error
 * Create by zzr  2015-05-05
 *******************************************************************************/
int vos_MemInit(UINT32 MemSize_U32)
{
    int RetCode_I32=0;
    INT32 i=0, j=0, MaxNo_I32=0;
    UINT32 BlockSize_aU32[MEM_NBLOCKSIZES] = VOS_MEM_BLOCKSIZES;    /* different block sizes */
    UINT32 PreAllocate_aU32[MEM_NBLOCKSIZES] = MEM_PREALLOCATE; /* number of blocks that should be pre-allocated */
    BYTE *TempBuf_aU8[MEM_MAX_PREALLOCATE] = {NULL};
    G_VOS_MEM_CTL.Area_pU8 = (BYTE *)malloc(MemSize_U32);
    if((NULL == G_VOS_MEM_CTL.Area_pU8 ) || ((UINT32)G_VOS_MEM_CTL.Area_pU8  % 4 != 0))
    {
        VOS_PRINT0(VOS_DBG_MEM, "init memory failed\n");
        return RET_MALLOC_ERR;
    }
    G_VOS_MEM_CTL.FreeArea_pU8 = G_VOS_MEM_CTL.Area_pU8;
    G_VOS_MEM_CTL.MemSize_U32 = MemSize_U32;
    G_VOS_MEM_CTL.AllocSize_U32 = 0;    /* allocated memory is 0 at beginning */
    G_VOS_MEM_CTL.NoOfBlocks_U32 = 0U;
    /* memeroy statistic init */
    G_VOS_MEM_CTL.MemCnt_ST.FreeSize_U32 = MemSize_U32;
    G_VOS_MEM_CTL.MemCnt_ST.MinFreeSize_U32= MemSize_U32;
    G_VOS_MEM_CTL.MemCnt_ST.AllocCnt_U32 = 0U;
    G_VOS_MEM_CTL.MemCnt_ST.AllocErrCnt_U32 = 0U;
    G_VOS_MEM_CTL.MemCnt_ST.FreeErrCnt_U32 = 0U;
    /* Create memory allocation semaphore */
    RetCode_I32 = vos_CreateSem(&G_VOS_MEM_CTL.Sem_I32, VOS_SEM_FULL);
    for(i = 0; i < MEM_NBLOCKSIZES; i++)
    {
        BlockSize_aU32[i] = ((BlockSize_aU32[i] + 3) / 4) * 4;  /* align size */
    }
    if(0 == G_VOS_MEM_CTL.NoOfBlocks_U32)
    {
        G_VOS_MEM_CTL.NoOfBlocks_U32 = MEM_NBLOCKSIZES;
        /* initialize free blocks header */
        for(i = 0; i < MEM_NBLOCKSIZES; i++)
        {
            G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST = (VOS_MEM_BLOCK *)NULL;
            G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32 = BlockSize_aU32[i];
            MaxNo_I32 = PreAllocate_aU32[i];
            if(MaxNo_I32 > MEM_MAX_PREALLOCATE)
            {
                MaxNo_I32 = MEM_MAX_PREALLOCATE;
            }
            /* pre-allocate max(the parameter) blocks of the area for each different size */
            for(j = 0; j < MaxNo_I32; j++)
            {
                TempBuf_aU8[j] = vos_MemAlloc(BlockSize_aU32[i]);
            }
            /* then free the area */
            for(j = 0; j < MaxNo_I32; j++)
            {
                vos_MemFree(TempBuf_aU8[j]);
            }
        }
    }
    return RetCode_I32;
}

/******************************************************************************
 * Name: void vos_MemDestroy(void)
 * Function:   shutdown memory are
 * Input:  non
 * return :non
 * Create by zzr  2015-05-05
 *******************************************************************************/
void vos_MemDestroy(void)
{
    vos_DestroySem(&G_VOS_MEM_CTL.Sem_I32);
}

/******************************************************************************
* Name: vos_MemAlloc
* Function:   allocate memory and return allocated buffer size
* Input: Size_U32, size of wanted memory area
* return :pointer to memory area, of NULL if error.
* Create by zzr  2015-05-05
*******************************************************************************/
BYTE *vos_MemAlloc(UINT32 Size_U32)
{
    UINT32 BlockSize_U32;
    BYTE *Buf_pU8 = NULL;
    Buf_pU8 = vos_MemAllocBuf(Size_U32, &BlockSize_U32);
    return(Buf_pU8);
}

/******************************************************************************
* Name: INT32 vos_MemFree(const BYTE *Allocated_pU8)
* Function:  free allocated memory  the freed memory block is added to a free
*            block linked list, for future use.
* Input:   Allocated_pU8,pointer to memory block to free
* return :   0 if OK, !=0 if not
* Create by zzr  2015-05-05
*******************************************************************************/
INT32 vos_MemFree(const BYTE *Allocated_pU8)
{
    int RetCode_I32 = RET_OK;
    UINT32 i;
    UINT32 BlockSize_U32;
    VOS_MEM_BLOCK *Block_pST = NULL;
    if(NULL == Allocated_pU8)
    {
        G_VOS_MEM_CTL.MemCnt_ST.FreeErrCnt_U32++;
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemFree parameter error : null pointer\n");
        return RET_ERR;
    }
    /* check that the returned memory is within the allocated area */
    if((Allocated_pU8 < G_VOS_MEM_CTL.Area_pU8) || (Allocated_pU8 >= (G_VOS_MEM_CTL.Area_pU8 + G_VOS_MEM_CTL.MemSize_U32)))
    {
        G_VOS_MEM_CTL.MemCnt_ST.FreeErrCnt_U32++;
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemFree ERROR returned memory not within allocated memory\n");
        return RET_ERR;
    }
    /* get memory semaphore */
    if(RET_OK != vos_GetSem(&G_VOS_MEM_CTL.Sem_I32, VOS_SEM_WAIT_FOREVER))
    {
        G_VOS_MEM_CTL.MemCnt_ST.FreeErrCnt_U32++;
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemFree can't get semaphore\n");
        return RET_ERR;
    }
    /* set block pointer to start of block, before the returned pointer */
    Block_pST = (VOS_MEM_BLOCK *)((const BYTE *)Allocated_pU8 - sizeof(VOS_MEM_BLOCK)); /* lint !e826 get pointer to allocated block */
    BlockSize_U32 = Block_pST->Size_U32;
    /* find appropriate free block item */
    for(i = 0; i < G_VOS_MEM_CTL.NoOfBlocks_U32; i++)
    {
        if(G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32 == BlockSize_U32)
        {
            break;
        }
    }
    if(i >= G_VOS_MEM_CTL.NoOfBlocks_U32)
    {
        G_VOS_MEM_CTL.MemCnt_ST.FreeErrCnt_U32++;
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemFree illegal sized memory \n");
        RetCode_I32 = RET_ERR;  /* try to return a illegal sized memory */
    }
    else
    {
        G_VOS_MEM_CTL.MemCnt_ST.FreeSize_U32 += BlockSize_U32 + sizeof(VOS_MEM_BLOCK);
        G_VOS_MEM_CTL.MemCnt_ST.AllocCnt_U32 --;
        /* put the returned block first in the linked list
         * pFirst point to the first block which has been freed latest, then the pBlock->pNext point to
         * the next block which has been freed the second latest(if only one has been freed, point to NULL) */
        Block_pST->Next_pST = G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST;
        G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST = Block_pST;
        /* destroy the size first in the block, if user tries to return same memory this will then fail */
        Block_pST->Size_U32 = 0;
    }
    /* release semaphore */
    if(RET_OK != vos_PutSem(&G_VOS_MEM_CTL.Sem_I32))
    {
        VOS_PRINT0(VOS_DBG_ERR, "vos_MemFree give semaphore ERROR\n");
    }
    return RetCode_I32;
}

/******************************************************************************
* Name: vos_MemAllocCntGet
* Function:  get the current value of allocated buffers.
* Input:
* return :  current value of allocated buffers
* Create by zzr  2015-05-05
*******************************************************************************/
UINT32 vos_MemAllocCntGet(void)
{
    return (G_VOS_MEM_CTL.MemCnt_ST.AllocCnt_U32);
}
/******************************************************************************
* Name: vos_MemShow
* Function:  displays the momory usage and statistic
* Input:     non
* return :  non
* Create by zzr  2015-05-05
*******************************************************************************/
void vos_MemShow(void)
{
    UINT32 i, j, k;
    VOS_MEM_BLOCK *Block_pST = NULL;
    VOS_PRINT0(VOS_DBG_INFO, "=============VOS MEM SHOW=============");
    VOS_PRINT1(VOS_DBG_INFO, "====Memory size = %d", G_VOS_MEM_CTL.MemSize_U32);
    VOS_PRINT1(VOS_DBG_INFO, "====Size allocated for blocks = %d", G_VOS_MEM_CTL.AllocSize_U32);
    VOS_PRINT1(VOS_DBG_INFO, "====Size not allocated for blocks = %d", G_VOS_MEM_CTL.MemSize_U32 -G_VOS_MEM_CTL.AllocSize_U32);
    VOS_PRINT1(VOS_DBG_INFO, "====Free size = %d", G_VOS_MEM_CTL.MemCnt_ST.FreeSize_U32);
    VOS_PRINT1(VOS_DBG_INFO, "====Minimum free size = %d",G_VOS_MEM_CTL.MemCnt_ST.MinFreeSize_U32);
    VOS_PRINT1(VOS_DBG_INFO, "====No of alloc errors = %d",G_VOS_MEM_CTL.MemCnt_ST.AllocErrCnt_U32);
    VOS_PRINT1(VOS_DBG_INFO, "====No of free errors = %d", G_VOS_MEM_CTL.MemCnt_ST.FreeErrCnt_U32);
    k = 0;
    for(i = 0; i < G_VOS_MEM_CTL.NoOfBlocks_U32; i++)
    {
        Block_pST =G_VOS_MEM_CTL.FreeBlock_aST[i].First_pST;
        j = 0;
        /* check if there is a free block ready */
        while(NULL != Block_pST)
        {
            j++;
            Block_pST = Block_pST->Next_pST;
        }
        k += G_VOS_MEM_CTL.MemCnt_ST.BlockCnt_aU32[i];
        /* this will print the total number of blocks , total number of freed blocks of
         * each different size */
        VOS_PRINT3(VOS_DBG_INFO, "******Block size = %d, counter = %d, free = %d",
                   G_VOS_MEM_CTL.FreeBlock_aST[i].Size_U32, G_VOS_MEM_CTL.MemCnt_ST.BlockCnt_aU32[i], j);
    }
    VOS_PRINT2(VOS_DBG_INFO, "====Total number of blocks = %d. Allocated blocks = %d\n", k,G_VOS_MEM_CTL.MemCnt_ST.AllocCnt_U32);
}

/******************************************************************************
* Name: vos_MemBelongVos
* Function:   checks if a pointer is a pointer into the TRDP vos memory area.
* Input:    Data_pU8,the memory pointer
* return :  TRUE if it is, FALSE if it isn't
* Create by zzr  2015-05-05
*******************************************************************************/
INT32 vos_MemBelongVos(const BYTE *Data_pU8)
{
    VOS_PRINT2(VOS_DBG_INFO,"VOS memory address:%#x, total size:%u",
               G_VOS_MEM_CTL.Area_pU8, G_VOS_MEM_CTL.MemSize_U32);
    return ((Data_pU8 >=G_VOS_MEM_CTL.Area_pU8) && (Data_pU8 < (G_VOS_MEM_CTL.Area_pU8 + G_VOS_MEM_CTL.MemSize_U32)));
}
