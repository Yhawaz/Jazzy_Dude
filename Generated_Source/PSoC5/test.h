/*******************************************************************************
* File Name: test.h  
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

#if !defined(CY_WaveDAC8_test_H) 
#define CY_WaveDAC8_test_H

#include "cytypes.h"
#include "cyfitter.h"
#include <test_Wave1_DMA_dma.h>
#include <test_Wave2_DMA_dma.h>
#include <test_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define test_WAVE1_TYPE     (0u)     /* Waveform for wave1 */
#define test_WAVE2_TYPE     (2u)     /* Waveform for wave2 */
#define test_SINE_WAVE      (0u)
#define test_SQUARE_WAVE    (1u)
#define test_TRIANGLE_WAVE  (2u)
#define test_SAWTOOTH_WAVE  (3u)
#define test_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define test_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define test_WAVE1_LENGTH   (100u)   /* Length for wave1 */
#define test_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define test_DEFAULT_RANGE    (0u) /* Default DAC range */
#define test_DAC_RANGE_1V     (0u)
#define test_DAC_RANGE_1V_BUF (16u)
#define test_DAC_RANGE_4V     (1u)
#define test_DAC_RANGE_4V_BUF (17u)
#define test_VOLT_MODE        (0u)
#define test_CURRENT_MODE     (1u)
#define test_DAC_MODE         (((test_DEFAULT_RANGE == test_DAC_RANGE_1V) || \
									  (test_DEFAULT_RANGE == test_DAC_RANGE_4V) || \
							  		  (test_DEFAULT_RANGE == test_DAC_RANGE_1V_BUF) || \
									  (test_DEFAULT_RANGE == test_DAC_RANGE_4V_BUF)) ? \
									   test_VOLT_MODE : test_CURRENT_MODE)

#define test_DACMODE test_DAC_MODE /* legacy definition for backward compatibility */

#define test_DIRECT_MODE (0u)
#define test_BUFFER_MODE (1u)
#define test_OUT_MODE    (((test_DEFAULT_RANGE == test_DAC_RANGE_1V_BUF) || \
								 (test_DEFAULT_RANGE == test_DAC_RANGE_4V_BUF)) ? \
								  test_BUFFER_MODE : test_DIRECT_MODE)

#if(test_OUT_MODE == test_BUFFER_MODE)
    #include <test_BuffAmp.h>
#endif /* test_OUT_MODE == test_BUFFER_MODE */

#define test_CLOCK_INT      (1u)
#define test_CLOCK_EXT      (0u)
#define test_CLOCK_SRC      (1u)

#if(test_CLOCK_SRC == test_CLOCK_INT)  
	#include <test_DacClk.h>
	#if defined(test_DacClk_PHASE)
		#define test_CLK_PHASE_0nS (1u)
	#endif /* defined(test_DacClk_PHASE) */
#endif /* test_CLOCK_SRC == test_CLOCK_INT */

#if (CY_PSOC3)
	#define test_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define test_Wave1_DMA_BYTES_PER_BURST      (1u)
#define test_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define test_Wave2_DMA_BYTES_PER_BURST      (1u)
#define test_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}test_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void test_Start(void)             ;
void test_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void test_Init(void)              ;
void test_Enable(void)            ;
void test_Stop(void)              ;

void test_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void test_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void test_Sleep(void)             ;
void test_Wakeup(void)            ;

#define test_SetSpeed       test_VDAC8_SetSpeed
#define test_SetRange       test_VDAC8_SetRange
#define test_SetValue       test_VDAC8_SetValue
#define test_DacTrim        test_VDAC8_DacTrim
#define test_SaveConfig     test_VDAC8_SaveConfig
#define test_RestoreConfig  test_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 test_initVar;

extern const uint8 CYCODE test_wave1[test_WAVE1_LENGTH];
extern const uint8 CYCODE test_wave2[test_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(test_DAC_MODE == test_VOLT_MODE)
    #define test_RANGE_1V       (0x00u)
    #define test_RANGE_4V       (0x04u)
#else /* current mode */
    #define test_RANGE_32uA     (0x00u)
    #define test_RANGE_255uA    (0x04u)
    #define test_RANGE_2mA      (0x08u)
    #define test_RANGE_2048uA   test_RANGE_2mA
#endif /* test_DAC_MODE == test_VOLT_MODE */

/* Power setting for SetSpeed API */
#define test_LOWSPEED       (0x00u)
#define test_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define test_DAC8__D test_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define test_HS_MASK        (0x02u)
#define test_HS_LOWPOWER    (0x00u)
#define test_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define test_MODE_MASK      (0x10u)
#define test_MODE_V         (0x00u)
#define test_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define test_RANGE_MASK     (0x0Cu)
#define test_RANGE_0        (0x00u)
#define test_RANGE_1        (0x04u)
#define test_RANGE_2        (0x08u)
#define test_RANGE_3        (0x0Cu)
#define test_IDIR_MASK      (0x04u)

#define test_DAC_RANGE      ((uint8)(0u << 2u) & test_RANGE_MASK)
#define test_DAC_POL        ((uint8)(0u >> 1u) & test_IDIR_MASK)


#endif /* CY_WaveDAC8_test_H  */

/* [] END OF FILE */
