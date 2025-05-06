/*******************************************************************************
* File Name: dac.h  
* Version 2.10
*
* Description:
*  This file contains the function prototypes and constants used in
*  the 8-bit Waveform DAC (WaveDAC8) Component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_WaveDAC8_dac_H) 
#define CY_WaveDAC8_dac_H

#include "cytypes.h"
#include "cyfitter.h"
#include <dac_Wave1_DMA_dma.h>
#include <dac_Wave2_DMA_dma.h>
#include <dac_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define dac_WAVE1_TYPE     (11u)     /* Waveform for wave1 */
#define dac_WAVE2_TYPE     (2u)     /* Waveform for wave2 */
#define dac_SINE_WAVE      (0u)
#define dac_SQUARE_WAVE    (1u)
#define dac_TRIANGLE_WAVE  (2u)
#define dac_SAWTOOTH_WAVE  (3u)
#define dac_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define dac_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define dac_WAVE1_LENGTH   (34u)   /* Length for wave1 */
#define dac_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define dac_DEFAULT_RANGE    (0u) /* Default DAC range */
#define dac_DAC_RANGE_1V     (0u)
#define dac_DAC_RANGE_1V_BUF (16u)
#define dac_DAC_RANGE_4V     (1u)
#define dac_DAC_RANGE_4V_BUF (17u)
#define dac_VOLT_MODE        (0u)
#define dac_CURRENT_MODE     (1u)
#define dac_DAC_MODE         (((dac_DEFAULT_RANGE == dac_DAC_RANGE_1V) || \
									  (dac_DEFAULT_RANGE == dac_DAC_RANGE_4V) || \
							  		  (dac_DEFAULT_RANGE == dac_DAC_RANGE_1V_BUF) || \
									  (dac_DEFAULT_RANGE == dac_DAC_RANGE_4V_BUF)) ? \
									   dac_VOLT_MODE : dac_CURRENT_MODE)

#define dac_DACMODE dac_DAC_MODE /* legacy definition for backward compatibility */

#define dac_DIRECT_MODE (0u)
#define dac_BUFFER_MODE (1u)
#define dac_OUT_MODE    (((dac_DEFAULT_RANGE == dac_DAC_RANGE_1V_BUF) || \
								 (dac_DEFAULT_RANGE == dac_DAC_RANGE_4V_BUF)) ? \
								  dac_BUFFER_MODE : dac_DIRECT_MODE)

#if(dac_OUT_MODE == dac_BUFFER_MODE)
    #include <dac_BuffAmp.h>
#endif /* dac_OUT_MODE == dac_BUFFER_MODE */

#define dac_CLOCK_INT      (1u)
#define dac_CLOCK_EXT      (0u)
#define dac_CLOCK_SRC      (0u)

#if(dac_CLOCK_SRC == dac_CLOCK_INT)  
	#include <dac_DacClk.h>
	#if defined(dac_DacClk_PHASE)
		#define dac_CLK_PHASE_0nS (1u)
	#endif /* defined(dac_DacClk_PHASE) */
#endif /* dac_CLOCK_SRC == dac_CLOCK_INT */

#if (CY_PSOC3)
	#define dac_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define dac_Wave1_DMA_BYTES_PER_BURST      (1u)
#define dac_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define dac_Wave2_DMA_BYTES_PER_BURST      (1u)
#define dac_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}dac_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void dac_Start(void)             ;
void dac_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void dac_Init(void)              ;
void dac_Enable(void)            ;
void dac_Stop(void)              ;

void dac_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void dac_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void dac_Sleep(void)             ;
void dac_Wakeup(void)            ;

#define dac_SetSpeed       dac_VDAC8_SetSpeed
#define dac_SetRange       dac_VDAC8_SetRange
#define dac_SetValue       dac_VDAC8_SetValue
#define dac_DacTrim        dac_VDAC8_DacTrim
#define dac_SaveConfig     dac_VDAC8_SaveConfig
#define dac_RestoreConfig  dac_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 dac_initVar;

extern const uint8 CYCODE dac_wave1[dac_WAVE1_LENGTH];
extern const uint8 CYCODE dac_wave2[dac_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(dac_DAC_MODE == dac_VOLT_MODE)
    #define dac_RANGE_1V       (0x00u)
    #define dac_RANGE_4V       (0x04u)
#else /* current mode */
    #define dac_RANGE_32uA     (0x00u)
    #define dac_RANGE_255uA    (0x04u)
    #define dac_RANGE_2mA      (0x08u)
    #define dac_RANGE_2048uA   dac_RANGE_2mA
#endif /* dac_DAC_MODE == dac_VOLT_MODE */

/* Power setting for SetSpeed API */
#define dac_LOWSPEED       (0x00u)
#define dac_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define dac_DAC8__D dac_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define dac_HS_MASK        (0x02u)
#define dac_HS_LOWPOWER    (0x00u)
#define dac_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define dac_MODE_MASK      (0x10u)
#define dac_MODE_V         (0x00u)
#define dac_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define dac_RANGE_MASK     (0x0Cu)
#define dac_RANGE_0        (0x00u)
#define dac_RANGE_1        (0x04u)
#define dac_RANGE_2        (0x08u)
#define dac_RANGE_3        (0x0Cu)
#define dac_IDIR_MASK      (0x04u)

#define dac_DAC_RANGE      ((uint8)(0u << 2u) & dac_RANGE_MASK)
#define dac_DAC_POL        ((uint8)(0u >> 1u) & dac_IDIR_MASK)


#endif /* CY_WaveDAC8_dac_H  */

/* [] END OF FILE */
