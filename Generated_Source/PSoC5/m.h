/*******************************************************************************
* File Name: m.h  
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

#if !defined(CY_WaveDAC8_m_H) 
#define CY_WaveDAC8_m_H

#include "cytypes.h"
#include "cyfitter.h"
#include <m_Wave1_DMA_dma.h>
#include <m_Wave2_DMA_dma.h>
#include <m_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define m_WAVE1_TYPE     (0u)     /* Waveform for wave1 */
#define m_WAVE2_TYPE     (2u)     /* Waveform for wave2 */
#define m_SINE_WAVE      (0u)
#define m_SQUARE_WAVE    (1u)
#define m_TRIANGLE_WAVE  (2u)
#define m_SAWTOOTH_WAVE  (3u)
#define m_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define m_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define m_WAVE1_LENGTH   (100u)   /* Length for wave1 */
#define m_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define m_DEFAULT_RANGE    (0u) /* Default DAC range */
#define m_DAC_RANGE_1V     (0u)
#define m_DAC_RANGE_1V_BUF (16u)
#define m_DAC_RANGE_4V     (1u)
#define m_DAC_RANGE_4V_BUF (17u)
#define m_VOLT_MODE        (0u)
#define m_CURRENT_MODE     (1u)
#define m_DAC_MODE         (((m_DEFAULT_RANGE == m_DAC_RANGE_1V) || \
									  (m_DEFAULT_RANGE == m_DAC_RANGE_4V) || \
							  		  (m_DEFAULT_RANGE == m_DAC_RANGE_1V_BUF) || \
									  (m_DEFAULT_RANGE == m_DAC_RANGE_4V_BUF)) ? \
									   m_VOLT_MODE : m_CURRENT_MODE)

#define m_DACMODE m_DAC_MODE /* legacy definition for backward compatibility */

#define m_DIRECT_MODE (0u)
#define m_BUFFER_MODE (1u)
#define m_OUT_MODE    (((m_DEFAULT_RANGE == m_DAC_RANGE_1V_BUF) || \
								 (m_DEFAULT_RANGE == m_DAC_RANGE_4V_BUF)) ? \
								  m_BUFFER_MODE : m_DIRECT_MODE)

#if(m_OUT_MODE == m_BUFFER_MODE)
    #include <m_BuffAmp.h>
#endif /* m_OUT_MODE == m_BUFFER_MODE */

#define m_CLOCK_INT      (1u)
#define m_CLOCK_EXT      (0u)
#define m_CLOCK_SRC      (1u)

#if(m_CLOCK_SRC == m_CLOCK_INT)  
	#include <m_DacClk.h>
	#if defined(m_DacClk_PHASE)
		#define m_CLK_PHASE_0nS (1u)
	#endif /* defined(m_DacClk_PHASE) */
#endif /* m_CLOCK_SRC == m_CLOCK_INT */

#if (CY_PSOC3)
	#define m_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define m_Wave1_DMA_BYTES_PER_BURST      (1u)
#define m_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define m_Wave2_DMA_BYTES_PER_BURST      (1u)
#define m_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}m_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void m_Start(void)             ;
void m_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void m_Init(void)              ;
void m_Enable(void)            ;
void m_Stop(void)              ;

void m_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void m_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void m_Sleep(void)             ;
void m_Wakeup(void)            ;

#define m_SetSpeed       m_VDAC8_SetSpeed
#define m_SetRange       m_VDAC8_SetRange
#define m_SetValue       m_VDAC8_SetValue
#define m_DacTrim        m_VDAC8_DacTrim
#define m_SaveConfig     m_VDAC8_SaveConfig
#define m_RestoreConfig  m_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 m_initVar;

extern const uint8 CYCODE m_wave1[m_WAVE1_LENGTH];
extern const uint8 CYCODE m_wave2[m_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(m_DAC_MODE == m_VOLT_MODE)
    #define m_RANGE_1V       (0x00u)
    #define m_RANGE_4V       (0x04u)
#else /* current mode */
    #define m_RANGE_32uA     (0x00u)
    #define m_RANGE_255uA    (0x04u)
    #define m_RANGE_2mA      (0x08u)
    #define m_RANGE_2048uA   m_RANGE_2mA
#endif /* m_DAC_MODE == m_VOLT_MODE */

/* Power setting for SetSpeed API */
#define m_LOWSPEED       (0x00u)
#define m_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define m_DAC8__D m_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define m_HS_MASK        (0x02u)
#define m_HS_LOWPOWER    (0x00u)
#define m_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define m_MODE_MASK      (0x10u)
#define m_MODE_V         (0x00u)
#define m_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define m_RANGE_MASK     (0x0Cu)
#define m_RANGE_0        (0x00u)
#define m_RANGE_1        (0x04u)
#define m_RANGE_2        (0x08u)
#define m_RANGE_3        (0x0Cu)
#define m_IDIR_MASK      (0x04u)

#define m_DAC_RANGE      ((uint8)(0u << 2u) & m_RANGE_MASK)
#define m_DAC_POL        ((uint8)(0u >> 1u) & m_IDIR_MASK)


#endif /* CY_WaveDAC8_m_H  */

/* [] END OF FILE */
