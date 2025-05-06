/*******************************************************************************
* File Name: mm_DacClk.h
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

#if !defined(CY_CLOCK_mm_DacClk_H)
#define CY_CLOCK_mm_DacClk_H

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

void mm_DacClk_Start(void) ;
void mm_DacClk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void mm_DacClk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void mm_DacClk_StandbyPower(uint8 state) ;
void mm_DacClk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 mm_DacClk_GetDividerRegister(void) ;
void mm_DacClk_SetModeRegister(uint8 modeBitMask) ;
void mm_DacClk_ClearModeRegister(uint8 modeBitMask) ;
uint8 mm_DacClk_GetModeRegister(void) ;
void mm_DacClk_SetSourceRegister(uint8 clkSource) ;
uint8 mm_DacClk_GetSourceRegister(void) ;
#if defined(mm_DacClk__CFG3)
void mm_DacClk_SetPhaseRegister(uint8 clkPhase) ;
uint8 mm_DacClk_GetPhaseRegister(void) ;
#endif /* defined(mm_DacClk__CFG3) */

#define mm_DacClk_Enable()                       mm_DacClk_Start()
#define mm_DacClk_Disable()                      mm_DacClk_Stop()
#define mm_DacClk_SetDivider(clkDivider)         mm_DacClk_SetDividerRegister(clkDivider, 1u)
#define mm_DacClk_SetDividerValue(clkDivider)    mm_DacClk_SetDividerRegister((clkDivider) - 1u, 1u)
#define mm_DacClk_SetMode(clkMode)               mm_DacClk_SetModeRegister(clkMode)
#define mm_DacClk_SetSource(clkSource)           mm_DacClk_SetSourceRegister(clkSource)
#if defined(mm_DacClk__CFG3)
#define mm_DacClk_SetPhase(clkPhase)             mm_DacClk_SetPhaseRegister(clkPhase)
#define mm_DacClk_SetPhaseValue(clkPhase)        mm_DacClk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(mm_DacClk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define mm_DacClk_CLKEN              (* (reg8 *) mm_DacClk__PM_ACT_CFG)
#define mm_DacClk_CLKEN_PTR          ((reg8 *) mm_DacClk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define mm_DacClk_CLKSTBY            (* (reg8 *) mm_DacClk__PM_STBY_CFG)
#define mm_DacClk_CLKSTBY_PTR        ((reg8 *) mm_DacClk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define mm_DacClk_DIV_LSB            (* (reg8 *) mm_DacClk__CFG0)
#define mm_DacClk_DIV_LSB_PTR        ((reg8 *) mm_DacClk__CFG0)
#define mm_DacClk_DIV_PTR            ((reg16 *) mm_DacClk__CFG0)

/* Clock MSB divider configuration register. */
#define mm_DacClk_DIV_MSB            (* (reg8 *) mm_DacClk__CFG1)
#define mm_DacClk_DIV_MSB_PTR        ((reg8 *) mm_DacClk__CFG1)

/* Mode and source configuration register */
#define mm_DacClk_MOD_SRC            (* (reg8 *) mm_DacClk__CFG2)
#define mm_DacClk_MOD_SRC_PTR        ((reg8 *) mm_DacClk__CFG2)

#if defined(mm_DacClk__CFG3)
/* Analog clock phase configuration register */
#define mm_DacClk_PHASE              (* (reg8 *) mm_DacClk__CFG3)
#define mm_DacClk_PHASE_PTR          ((reg8 *) mm_DacClk__CFG3)
#endif /* defined(mm_DacClk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define mm_DacClk_CLKEN_MASK         mm_DacClk__PM_ACT_MSK
#define mm_DacClk_CLKSTBY_MASK       mm_DacClk__PM_STBY_MSK

/* CFG2 field masks */
#define mm_DacClk_SRC_SEL_MSK        mm_DacClk__CFG2_SRC_SEL_MASK
#define mm_DacClk_MODE_MASK          (~(mm_DacClk_SRC_SEL_MSK))

#if defined(mm_DacClk__CFG3)
/* CFG3 phase mask */
#define mm_DacClk_PHASE_MASK         mm_DacClk__CFG3_PHASE_DLY_MASK
#endif /* defined(mm_DacClk__CFG3) */

#endif /* CY_CLOCK_mm_DacClk_H */


/* [] END OF FILE */
