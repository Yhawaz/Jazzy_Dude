/******************************************************************************
* File Name: m_Wave1_DMA_dma.h  
* Version 1.70
*
*  Description:
*   Provides the function definitions for the DMA Controller.
*
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#if !defined(CY_DMA_m_Wave1_DMA_DMA_H__)
#define CY_DMA_m_Wave1_DMA_DMA_H__



#include <CYDMAC.H>
#include <CYFITTER.H>

#define m_Wave1_DMA__TD_TERMOUT_EN (((0 != m_Wave1_DMA__TERMOUT0_EN) ? TD_TERMOUT0_EN : 0) | \
    (m_Wave1_DMA__TERMOUT1_EN ? TD_TERMOUT1_EN : 0))

/* Zero based index of m_Wave1_DMA dma channel */
extern uint8 m_Wave1_DMA_DmaHandle;


uint8 m_Wave1_DMA_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress) ;
void  m_Wave1_DMA_DmaRelease(void) ;


/* CY_DMA_m_Wave1_DMA_DMA_H__ */
#endif
