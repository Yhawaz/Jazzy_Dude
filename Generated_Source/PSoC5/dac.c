/*******************************************************************************
* File Name: dac.c
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

#include "dac.h"

uint8  dac_initVar = 0u;

const uint8 CYCODE dac_wave1[dac_WAVE1_LENGTH] = { 102u,72u,38u,19u,12u,4u,0u,7u,15u,21u,30u,48u,69u,93u,116u,138u,158u,176u,184u,183u,186u,197u,212u,226u,231u,229u,221u,212u,204u,192u,183u,182u,186u,188u };
const uint8 CYCODE dac_wave2[dac_WAVE2_LENGTH] = { 128u,132u,137u,141u,146u,150u,155u,159u,164u,168u,173u,177u,182u,186u,191u,195u,200u,204u,209u,213u,218u,222u,227u,231u,236u,240u,236u,231u,227u,222u,218u,213u,209u,204u,200u,195u,191u,186u,182u,177u,173u,168u,164u,159u,155u,150u,146u,141u,137u,132u,128u,123u,118u,114u,109u,105u,100u,96u,91u,87u,82u,78u,73u,69u,64u,60u,55u,51u,46u,42u,37u,33u,28u,24u,19u,14u,19u,24u,28u,33u,37u,42u,46u,51u,55u,60u,64u,69u,73u,78u,82u,87u,91u,96u,100u,105u,109u,114u,118u,123u };

static uint8  dac_Wave1Chan;
static uint8  dac_Wave2Chan;
static uint8  dac_Wave1TD;
static uint8  dac_Wave2TD;


/*******************************************************************************
* Function Name: dac_Init
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
void dac_Init(void) 
{
	dac_VDAC8_Init();
	dac_VDAC8_SetSpeed(dac_HIGHSPEED);
	dac_VDAC8_SetRange(dac_DAC_RANGE);

	#if(dac_DAC_MODE == dac_CURRENT_MODE)
		dac_IDAC8_SetPolarity(dac_DAC_POL);
	#endif /* dac_DAC_MODE == dac_CURRENT_MODE */

	#if(dac_OUT_MODE == dac_BUFFER_MODE)
	   dac_BuffAmp_Init();
	#endif /* dac_OUT_MODE == dac_BUFFER_MODE */

	/* Get the TD Number for the DMA channel 1 and 2   */
	dac_Wave1TD = CyDmaTdAllocate();
	dac_Wave2TD = CyDmaTdAllocate();
	
	/* Initialize waveform pointers  */
	dac_Wave1Setup(dac_wave1, dac_WAVE1_LENGTH) ;
	dac_Wave2Setup(dac_wave2, dac_WAVE2_LENGTH) ;
	
	/* Initialize the internal clock if one present  */
	#if defined(dac_DacClk_PHASE)
	   dac_DacClk_SetPhase(dac_CLK_PHASE_0nS);
	#endif /* defined(dac_DacClk_PHASE) */
}


/*******************************************************************************
* Function Name: dac_Enable
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
void dac_Enable(void) 
{
	dac_VDAC8_Enable();

	#if(dac_OUT_MODE == dac_BUFFER_MODE)
	   dac_BuffAmp_Enable();
	#endif /* dac_OUT_MODE == dac_BUFFER_MODE */

	/* 
	* Enable the channel. It is configured to remember the TD value so that
	* it can be restored from the place where it has been stopped.
	*/
	(void)CyDmaChEnable(dac_Wave1Chan, 1u);
	(void)CyDmaChEnable(dac_Wave2Chan, 1u);
	
	/* set the initial value */
	dac_SetValue(0u);
	
	#if(dac_CLOCK_SRC == dac_CLOCK_INT)  	
	   dac_DacClk_Start();
	#endif /* dac_CLOCK_SRC == dac_CLOCK_INT */
}


/*******************************************************************************
* Function Name: dac_Start
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
void dac_Start(void) 
{
	/* If not Initialized then initialize all required hardware and software */
	if(dac_initVar == 0u)
	{
		dac_Init();
		dac_initVar = 1u;
	}
	
	dac_Enable();
}


/*******************************************************************************
* Function Name: dac_StartEx
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
void dac_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)

{
	dac_Wave1Setup(wavePtr1, sampleSize1);
	dac_Wave2Setup(wavePtr2, sampleSize2);
	dac_Start();
}


/*******************************************************************************
* Function Name: dac_Stop
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
void dac_Stop(void) 
{
	/* Turn off internal clock, if one present */
	#if(dac_CLOCK_SRC == dac_CLOCK_INT)  	
	   dac_DacClk_Stop();
	#endif /* dac_CLOCK_SRC == dac_CLOCK_INT */
	
	/* Disble DMA channels */
	(void)CyDmaChDisable(dac_Wave1Chan);
	(void)CyDmaChDisable(dac_Wave2Chan);

	/* Disable power to DAC */
	dac_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: dac_Wave1Setup
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
void dac_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)

{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (dac_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
		
		dac_Wave1Chan = dac_Wave1_DMA_DmaInitialize(
		dac_Wave1_DMA_BYTES_PER_BURST, dac_Wave1_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		dac_Wave1Chan = dac_Wave1_DMA_DmaInitialize(
		dac_Wave1_DMA_BYTES_PER_BURST, dac_Wave1_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(dac_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
    * Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(dac_Wave1TD, sampleSize, dac_Wave1TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)dac_Wave1_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(dac_Wave1TD, LO16((uint32)wavePtr), LO16(dac_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(dac_Wave1Chan, dac_Wave1TD);
}


/*******************************************************************************
* Function Name: dac_Wave2Setup
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
void dac_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
 
{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (dac_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
			
		dac_Wave2Chan = dac_Wave2_DMA_DmaInitialize(
		dac_Wave2_DMA_BYTES_PER_BURST, dac_Wave2_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		dac_Wave2Chan = dac_Wave2_DMA_DmaInitialize(
		dac_Wave2_DMA_BYTES_PER_BURST, dac_Wave2_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(dac_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
	* Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(dac_Wave2TD, sampleSize, dac_Wave2TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)dac_Wave2_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(dac_Wave2TD, LO16((uint32)wavePtr), LO16(dac_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(dac_Wave2Chan, dac_Wave2TD);
}


/* [] END OF FILE */
