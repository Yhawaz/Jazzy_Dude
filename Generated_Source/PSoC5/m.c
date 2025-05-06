/*******************************************************************************
* File Name: m.c
* Version 2.10
*
* Description:
*  This file provides the source code for the 8-bit Waveform DAC 
*  (WaveDAC8) Component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "m.h"

uint8  m_initVar = 0u;

const uint8 CYCODE m_wave1[m_WAVE1_LENGTH] = { 128u,135u,143u,151u,159u,166u,174u,181u,188u,194u,201u,207u,213u,219u,224u,229u,233u,237u,241u,244u,246u,249u,250u,252u,252u,252u,252u,252u,250u,249u,246u,244u,241u,237u,233u,229u,224u,219u,213u,207u,201u,194u,188u,181u,174u,166u,159u,151u,143u,135u,128u,120u,112u,104u,96u,89u,81u,74u,67u,61u,54u,48u,42u,36u,31u,26u,22u,18u,14u,11u,9u,6u,5u,3u,3u,3u,3u,3u,5u,6u,9u,11u,14u,18u,22u,26u,31u,36u,42u,48u,54u,61u,67u,74u,81u,89u,96u,104u,112u,120u };
const uint8 CYCODE m_wave2[m_WAVE2_LENGTH] = { 128u,133u,138u,143u,148u,153u,158u,163u,168u,173u,178u,184u,189u,194u,199u,204u,209u,214u,219u,224u,230u,235u,240u,245u,250u,255u,250u,245u,240u,235u,230u,224u,219u,214u,209u,204u,199u,194u,189u,184u,178u,173u,168u,163u,158u,153u,148u,143u,138u,133u,128u,122u,117u,112u,107u,102u,97u,92u,87u,82u,76u,71u,66u,61u,56u,51u,46u,41u,36u,31u,25u,20u,15u,10u,5u,0u,5u,10u,15u,20u,25u,31u,36u,41u,46u,51u,56u,61u,66u,71u,77u,82u,87u,92u,97u,102u,107u,112u,117u,122u };

static uint8  m_Wave1Chan;
static uint8  m_Wave2Chan;
static uint8  m_Wave1TD;
static uint8  m_Wave2TD;


/*******************************************************************************
* Function Name: m_Init
********************************************************************************
*
* Summary:
*  Initializes component with parameters set in the customizer.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void m_Init(void) 
{
	m_VDAC8_Init();
	m_VDAC8_SetSpeed(m_HIGHSPEED);
	m_VDAC8_SetRange(m_DAC_RANGE);

	#if(m_DAC_MODE == m_CURRENT_MODE)
		m_IDAC8_SetPolarity(m_DAC_POL);
	#endif /* m_DAC_MODE == m_CURRENT_MODE */

	#if(m_OUT_MODE == m_BUFFER_MODE)
	   m_BuffAmp_Init();
	#endif /* m_OUT_MODE == m_BUFFER_MODE */

	/* Get the TD Number for the DMA channel 1 and 2   */
	m_Wave1TD = CyDmaTdAllocate();
	m_Wave2TD = CyDmaTdAllocate();
	
	/* Initialize waveform pointers  */
	m_Wave1Setup(m_wave1, m_WAVE1_LENGTH) ;
	m_Wave2Setup(m_wave2, m_WAVE2_LENGTH) ;
	
	/* Initialize the internal clock if one present  */
	#if defined(m_DacClk_PHASE)
	   m_DacClk_SetPhase(m_CLK_PHASE_0nS);
	#endif /* defined(m_DacClk_PHASE) */
}


/*******************************************************************************
* Function Name: m_Enable
********************************************************************************
*  
* Summary: 
*  Enables the DAC block and DMA operation.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void m_Enable(void) 
{
	m_VDAC8_Enable();

	#if(m_OUT_MODE == m_BUFFER_MODE)
	   m_BuffAmp_Enable();
	#endif /* m_OUT_MODE == m_BUFFER_MODE */

	/* 
	* Enable the channel. It is configured to remember the TD value so that
	* it can be restored from the place where it has been stopped.
	*/
	(void)CyDmaChEnable(m_Wave1Chan, 1u);
	(void)CyDmaChEnable(m_Wave2Chan, 1u);
	
	/* set the initial value */
	m_SetValue(0u);
	
	#if(m_CLOCK_SRC == m_CLOCK_INT)  	
	   m_DacClk_Start();
	#endif /* m_CLOCK_SRC == m_CLOCK_INT */
}


/*******************************************************************************
* Function Name: m_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as 
*  executing the stop function.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void m_Start(void) 
{
	/* If not Initialized then initialize all required hardware and software */
	if(m_initVar == 0u)
	{
		m_Init();
		m_initVar = 1u;
	}
	
	m_Enable();
}


/*******************************************************************************
* Function Name: m_StartEx
********************************************************************************
*
* Summary:
*  The StartEx function sets pointers and sizes for both waveforms
*  and then starts the component.
*
* Parameters:  
*   uint8 * wavePtr1:     Pointer to the waveform 1 array.
*   uint16  sampleSize1:  The amount of samples in the waveform 1.
*   uint8 * wavePtr2:     Pointer to the waveform 2 array.
*   uint16  sampleSize2:  The amount of samples in the waveform 2.
*
* Return: 
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void m_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)

{
	m_Wave1Setup(wavePtr1, sampleSize1);
	m_Wave2Setup(wavePtr2, sampleSize2);
	m_Start();
}


/*******************************************************************************
* Function Name: m_Stop
********************************************************************************
*
* Summary:
*  Stops the clock (if internal), disables the DMA channels
*  and powers down the DAC.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
*******************************************************************************/
void m_Stop(void) 
{
	/* Turn off internal clock, if one present */
	#if(m_CLOCK_SRC == m_CLOCK_INT)  	
	   m_DacClk_Stop();
	#endif /* m_CLOCK_SRC == m_CLOCK_INT */
	
	/* Disble DMA channels */
	(void)CyDmaChDisable(m_Wave1Chan);
	(void)CyDmaChDisable(m_Wave2Chan);

	/* Disable power to DAC */
	m_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: m_Wave1Setup
********************************************************************************
*
* Summary:
*  Sets pointer and size for waveform 1.                                    
*
* Parameters:  
*  uint8 * WavePtr:     Pointer to the waveform array.
*  uint16  SampleSize:  The amount of samples in the waveform.
*
* Return: 
*  None 
*
*******************************************************************************/
void m_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)

{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (m_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
		
		m_Wave1Chan = m_Wave1_DMA_DmaInitialize(
		m_Wave1_DMA_BYTES_PER_BURST, m_Wave1_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		m_Wave1Chan = m_Wave1_DMA_DmaInitialize(
		m_Wave1_DMA_BYTES_PER_BURST, m_Wave1_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(m_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
    * Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(m_Wave1TD, sampleSize, m_Wave1TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)m_Wave1_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(m_Wave1TD, LO16((uint32)wavePtr), LO16(m_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(m_Wave1Chan, m_Wave1TD);
}


/*******************************************************************************
* Function Name: m_Wave2Setup
********************************************************************************
*
* Summary:
*  Sets pointer and size for waveform 2.                                    
*
* Parameters:  
*  uint8 * WavePtr:     Pointer to the waveform array.
*  uint16  SampleSize:  The amount of samples in the waveform.
*
* Return: 
*  None
*
*******************************************************************************/
void m_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
 
{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (m_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
			
		m_Wave2Chan = m_Wave2_DMA_DmaInitialize(
		m_Wave2_DMA_BYTES_PER_BURST, m_Wave2_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		m_Wave2Chan = m_Wave2_DMA_DmaInitialize(
		m_Wave2_DMA_BYTES_PER_BURST, m_Wave2_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(m_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
	* Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(m_Wave2TD, sampleSize, m_Wave2TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)m_Wave2_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(m_Wave2TD, LO16((uint32)wavePtr), LO16(m_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(m_Wave2Chan, m_Wave2TD);
}


/* [] END OF FILE */
