/*******************************************************************************
* File Name: mm.h  
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

#if !defined(CY_WaveDAC8_mm_H) 
#define CY_WaveDAC8_mm_H

#include "cytypes.h"
#include "cyfitter.h"
#include <mm_Wave1_DMA_dma.h>
#include <mm_Wave2_DMA_dma.h>
#include <mm_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define mm_WAVE1_TYPE     (0u)     /* Waveform for wave1 */
#define mm_WAVE2_TYPE     (2u)     /* Waveform for wave2 */
#define mm_SINE_WAVE      (0u)
#define mm_SQUARE_WAVE    (1u)
#define mm_TRIANGLE_WAVE  (2u)
#define mm_SAWTOOTH_WAVE  (3u)
#define mm_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define mm_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define mm_WAVE1_LENGTH   (100u)   /* Length for wave1 */
#define mm_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define mm_DEFAULT_RANGE    (0u) /* Default DAC range */
#define mm_DAC_RANGE_1V     (0u)
#define mm_DAC_RANGE_1V_BUF (16u)
#define mm_DAC_RANGE_4V     (1u)
#define mm_DAC_RANGE_4V_BUF (17u)
#define mm_VOLT_MODE        (0u)
#define mm_CURRENT_MODE     (1u)
#define mm_DAC_MODE         (((mm_DEFAULT_RANGE == mm_DAC_RANGE_1V) || \
									  (mm_DEFAULT_RANGE == mm_DAC_RANGE_4V) || \
							  		  (mm_DEFAULT_RANGE == mm_DAC_RANGE_1V_BUF) || \
									  (mm_DEFAULT_RANGE == mm_DAC_RANGE_4V_BUF)) ? \
									   mm_VOLT_MODE : mm_CURRENT_MODE)

#define mm_DACMODE mm_DAC_MODE /* legacy definition for backward compatibility */

#define mm_DIRECT_MODE (0u)
#define mm_BUFFER_MODE (1u)
#define mm_OUT_MODE    (((mm_DEFAULT_RANGE == mm_DAC_RANGE_1V_BUF) || \
								 (mm_DEFAULT_RANGE == mm_DAC_RANGE_4V_BUF)) ? \
								  mm_BUFFER_MODE : mm_DIRECT_MODE)

#if(mm_OUT_MODE == mm_BUFFER_MODE)
    #include <mm_BuffAmp.h>
#endif /* mm_OUT_MODE == mm_BUFFER_MODE */

#define mm_CLOCK_INT      (1u)
#define mm_CLOCK_EXT      (0u)
#define mm_CLOCK_SRC      (1u)

#if(mm_CLOCK_SRC == mm_CLOCK_INT)  
	#include <mm_DacClk.h>
	#if defined(mm_DacClk_PHASE)
		#define mm_CLK_PHASE_0nS (1u)
	#endif /* defined(mm_DacClk_PHASE) */
#endif /* mm_CLOCK_SRC == mm_CLOCK_INT */

#if (CY_PSOC3)
	#define mm_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define mm_Wave1_DMA_BYTES_PER_BURST      (1u)
#define mm_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define mm_Wave2_DMA_BYTES_PER_BURST      (1u)
#define mm_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}mm_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void mm_Start(void)             ;
void mm_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void mm_Init(void)              ;
void mm_Enable(void)            ;
void mm_Stop(void)              ;

void mm_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void mm_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void mm_Sleep(void)             ;
void mm_Wakeup(void)            ;

#define mm_SetSpeed       mm_VDAC8_SetSpeed
#define mm_SetRange       mm_VDAC8_SetRange
#define mm_SetValue       mm_VDAC8_SetValue
#define mm_DacTrim        mm_VDAC8_DacTrim
#define mm_SaveConfig     mm_VDAC8_SaveConfig
#define mm_RestoreConfig  mm_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 mm_initVar;

extern const uint8 CYCODE mm_wave1[mm_WAVE1_LENGTH];
extern const uint8 CYCODE mm_wave2[mm_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(mm_DAC_MODE == mm_VOLT_MODE)
    #define mm_RANGE_1V       (0x00u)
    #define mm_RANGE_4V       (0x04u)
#else /* current mode */
    #define mm_RANGE_32uA     (0x00u)
    #define mm_RANGE_255uA    (0x04u)
    #define mm_RANGE_2mA      (0x08u)
    #define mm_RANGE_2048uA   mm_RANGE_2mA
#endif /* mm_DAC_MODE == mm_VOLT_MODE */

/* Power setting for SetSpeed API */
#define mm_LOWSPEED       (0x00u)
#define mm_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define mm_DAC8__D mm_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define mm_HS_MASK        (0x02u)
#define mm_HS_LOWPOWER    (0x00u)
#define mm_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define mm_MODE_MASK      (0x10u)
#define mm_MODE_V         (0x00u)
#define mm_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define mm_RANGE_MASK     (0x0Cu)
#define mm_RANGE_0        (0x00u)
#define mm_RANGE_1        (0x04u)
#define mm_RANGE_2        (0x08u)
#define mm_RANGE_3        (0x0Cu)
#define mm_IDIR_MASK      (0x04u)

#define mm_DAC_RANGE      ((uint8)(0u << 2u) & mm_RANGE_MASK)
#define mm_DAC_POL        ((uint8)(0u >> 1u) & mm_IDIR_MASK)


#endif /* CY_WaveDAC8_mm_H  */

/* [] END OF FILE */
