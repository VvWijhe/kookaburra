/**
  ******************************************************************************
  * @file    usb_mem.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    28-August-2012
  * @brief   Utility functions for memory transfers to/from PMA
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include <cortex-m.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



#ifdef USB_GEBRUIK_DMA

static DMA_Channel_TypeDef * const dma = DMA1_Channel3;  /* VERANDER indien nodig */
static uint32_t DMA_IT = DMA1_IT_TC3;
static const uint8_t NVIC_IRQChannel = DMA1_Channel3_IRQn;
static KiwandaOSSemafoor  memDMATransferKlaar;

static void UserPMADMACopy(UInt16 const * const src,
               UInt16 * const dst,
               const Teller aantal)
{
   DMA_Cmd(dma, DISABLE);
    /* geef lengte,bron en bestemming en maak beschikbaar de dma */
    DMA_SetCurrDataCounter(dma,aantal);
    dma->CMAR = (UInt32) src;
    dma->CPAR = (UInt32) dst;

     /* Enable DMA Transfer Complete interrupt */
    DMA_ITConfig(dma, DMA_IT_TC, ENABLE);
    DMA_Cmd(dma, ENABLE);  /* Enable the transfer */

    FoutCode retcode = memDMATransferKlaar.wacht(500); /* wacht tot de semafoor wordt teruggeven */

    KiwandaAssert(retcode == Ok);

     /* interrupt en dma kanaal uit */
    DMA_ITConfig(dma, DMA_IT_TC, DISABLE);


}

void UserPMABufferCopyInit()
{
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

     /* DMA1 channel1 configuration ---------------------------------------------*/
     /* Enable DMA2 clock --------------------------------------------------------*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(dma);

    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(dma, &DMA_InitStructure);

     /* Enable DMA1 channel1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  /* een  hoger dan USB LP isr */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    memDMATransferKlaar.init(0,"USB Mem <-->PMA semafoor");

    const UInt16 testsrc[] = { 1,2,3,4,5,6,7,8,9,10 };
    UInt16 testdst[12];

    UserPMADMACopy(testsrc,testdst,10);

    KiwandaAssert(0 == memcmp(testsrc,testdst,10*sizeof(UInt16)));
}


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  This function handles DMA1 Channel 1 interrupt request.
 * @param  None
 * @retval None
 */
    void DMA1_Channel3_IRQHandler(void)
    {
    OSIntEnter();

     /* Test on DMA1 Channel1 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA_IT))
    {
         /* DMA1 finished the transfer of SrcBuffer */
         //  EndOfTransfer = 1;

         /* Clear DMA1 Channel1 Half Transfer, Transfer Complete and Global interrupt pending bits */
        DMA_ClearITPendingBit(DMA_IT);

        memDMATransferKlaar.geef();   /* geef aan wachtende taak aan dat de transfer klaar is */
    }
    OSIntExit();
    }

#ifdef __cplusplus
}
#endif




#endif


/*******************************************************************************
* Function Name  : UserToPMABufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf: pointer to user memory area.
*                  - wPMABufAddr: address into PMA.
*                  - wNBytes: no. of bytes to be copied.
* Output         : None.
* Return         : None	.
*******************************************************************************/
void UserToPMABufferCopy(uint8_t *pbUsrBuf, uint16_t wPMABufAddr, uint16_t wNBytes) {


    uint16_t *pdwVal = (uint16_t * )(wPMABufAddr * 2 + PMAAddr);
    const uint32_t n = (wNBytes + 1) >> 1;   /* n = (wNBytes + 1) / 2 */

#ifdef USB_GEBRUIK_DMA
    /* controleer of we op dit moment in een exeception handler (ISR) zijn */
        if (0 != CortexMProcessor::get_IPSR())  /* we worden aangeroepen uit een ISR */
        {
#endif

    for (uint32_t i = n; i != 0; i--) {
        const uint32_t temp1 = (uint16_t) * pbUsrBuf;
        pbUsrBuf++;
        const uint32_t temp2 = temp1 | (uint16_t) * pbUsrBuf << 8;
        *pdwVal++ = temp2;
        pdwVal++;
        pbUsrBuf++;
    }

#ifdef USB_GEBRUIK_DMA
    }
    else
    {
    UserPMADMACopy((UInt16 const * const)pbUsrBuf,
               pdwVal,
               n);

    }
#endif

}

/*******************************************************************************
* Function Name  : PMAToUserBufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf    = pointer to user memory area.
*                  - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
void PMAToUserBufferCopy(uint8_t *pbUsrBuf, uint16_t wPMABufAddr, uint16_t wNBytes) {
    const uint32_t n = (wNBytes + 1) >> 1;/* /2*/
    uint16_t *pbUsrBufPtr = (uint16_t *) pbUsrBuf;
    uint32_t const *pdwVal = (uint32_t * )(wPMABufAddr * 2 + PMAAddr);
    for (Teller i = n; i != 0; i--) {
        *(pbUsrBufPtr)++ = *pdwVal++;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
