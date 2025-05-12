/*******************************************************************************
* File Name: PITCH_ADC_theACLK.h
* Version 2.10
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_PITCH_ADC_theACLK_H)
#define CY_CLOCK_PITCH_ADC_theACLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void PITCH_ADC_theACLK_Start(void) ;
void PITCH_ADC_theACLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void PITCH_ADC_theACLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void PITCH_ADC_theACLK_StandbyPower(uint8 state) ;
void PITCH_ADC_theACLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 PITCH_ADC_theACLK_GetDividerRegister(void) ;
void PITCH_ADC_theACLK_SetModeRegister(uint8 modeBitMask) ;
void PITCH_ADC_theACLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 PITCH_ADC_theACLK_GetModeRegister(void) ;
void PITCH_ADC_theACLK_SetSourceRegister(uint8 clkSource) ;
uint8 PITCH_ADC_theACLK_GetSourceRegister(void) ;
#if defined(PITCH_ADC_theACLK__CFG3)
void PITCH_ADC_theACLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 PITCH_ADC_theACLK_GetPhaseRegister(void) ;
#endif /* defined(PITCH_ADC_theACLK__CFG3) */

#define PITCH_ADC_theACLK_Enable()                       PITCH_ADC_theACLK_Start()
#define PITCH_ADC_theACLK_Disable()                      PITCH_ADC_theACLK_Stop()
#define PITCH_ADC_theACLK_SetDivider(clkDivider)         PITCH_ADC_theACLK_SetDividerRegister(clkDivider, 1u)
#define PITCH_ADC_theACLK_SetDividerValue(clkDivider)    PITCH_ADC_theACLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define PITCH_ADC_theACLK_SetMode(clkMode)               PITCH_ADC_theACLK_SetModeRegister(clkMode)
#define PITCH_ADC_theACLK_SetSource(clkSource)           PITCH_ADC_theACLK_SetSourceRegister(clkSource)
#if defined(PITCH_ADC_theACLK__CFG3)
#define PITCH_ADC_theACLK_SetPhase(clkPhase)             PITCH_ADC_theACLK_SetPhaseRegister(clkPhase)
#define PITCH_ADC_theACLK_SetPhaseValue(clkPhase)        PITCH_ADC_theACLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(PITCH_ADC_theACLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define PITCH_ADC_theACLK_CLKEN              (* (reg8 *) PITCH_ADC_theACLK__PM_ACT_CFG)
#define PITCH_ADC_theACLK_CLKEN_PTR          ((reg8 *) PITCH_ADC_theACLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define PITCH_ADC_theACLK_CLKSTBY            (* (reg8 *) PITCH_ADC_theACLK__PM_STBY_CFG)
#define PITCH_ADC_theACLK_CLKSTBY_PTR        ((reg8 *) PITCH_ADC_theACLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define PITCH_ADC_theACLK_DIV_LSB            (* (reg8 *) PITCH_ADC_theACLK__CFG0)
#define PITCH_ADC_theACLK_DIV_LSB_PTR        ((reg8 *) PITCH_ADC_theACLK__CFG0)
#define PITCH_ADC_theACLK_DIV_PTR            ((reg16 *) PITCH_ADC_theACLK__CFG0)

/* Clock MSB divider configuration register. */
#define PITCH_ADC_theACLK_DIV_MSB            (* (reg8 *) PITCH_ADC_theACLK__CFG1)
#define PITCH_ADC_theACLK_DIV_MSB_PTR        ((reg8 *) PITCH_ADC_theACLK__CFG1)

/* Mode and source configuration register */
#define PITCH_ADC_theACLK_MOD_SRC            (* (reg8 *) PITCH_ADC_theACLK__CFG2)
#define PITCH_ADC_theACLK_MOD_SRC_PTR        ((reg8 *) PITCH_ADC_theACLK__CFG2)

#if defined(PITCH_ADC_theACLK__CFG3)
/* Analog clock phase configuration register */
#define PITCH_ADC_theACLK_PHASE              (* (reg8 *) PITCH_ADC_theACLK__CFG3)
#define PITCH_ADC_theACLK_PHASE_PTR          ((reg8 *) PITCH_ADC_theACLK__CFG3)
#endif /* defined(PITCH_ADC_theACLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define PITCH_ADC_theACLK_CLKEN_MASK         PITCH_ADC_theACLK__PM_ACT_MSK
#define PITCH_ADC_theACLK_CLKSTBY_MASK       PITCH_ADC_theACLK__PM_STBY_MSK

/* CFG2 field masks */
#define PITCH_ADC_theACLK_SRC_SEL_MSK        PITCH_ADC_theACLK__CFG2_SRC_SEL_MASK
#define PITCH_ADC_theACLK_MODE_MASK          (~(PITCH_ADC_theACLK_SRC_SEL_MSK))

#if defined(PITCH_ADC_theACLK__CFG3)
/* CFG3 phase mask */
#define PITCH_ADC_theACLK_PHASE_MASK         PITCH_ADC_theACLK__CFG3_PHASE_DLY_MASK
#endif /* defined(PITCH_ADC_theACLK__CFG3) */

#endif /* CY_CLOCK_PITCH_ADC_theACLK_H */


/* [] END OF FILE */
