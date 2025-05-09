/*******************************************************************************
* File Name: piano_clk.h
* Version 2.20
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

#if !defined(CY_CLOCK_piano_clk_H)
#define CY_CLOCK_piano_clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void piano_clk_Start(void) ;
void piano_clk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void piano_clk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void piano_clk_StandbyPower(uint8 state) ;
void piano_clk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 piano_clk_GetDividerRegister(void) ;
void piano_clk_SetModeRegister(uint8 modeBitMask) ;
void piano_clk_ClearModeRegister(uint8 modeBitMask) ;
uint8 piano_clk_GetModeRegister(void) ;
void piano_clk_SetSourceRegister(uint8 clkSource) ;
uint8 piano_clk_GetSourceRegister(void) ;
#if defined(piano_clk__CFG3)
void piano_clk_SetPhaseRegister(uint8 clkPhase) ;
uint8 piano_clk_GetPhaseRegister(void) ;
#endif /* defined(piano_clk__CFG3) */

#define piano_clk_Enable()                       piano_clk_Start()
#define piano_clk_Disable()                      piano_clk_Stop()
#define piano_clk_SetDivider(clkDivider)         piano_clk_SetDividerRegister(clkDivider, 1u)
#define piano_clk_SetDividerValue(clkDivider)    piano_clk_SetDividerRegister((clkDivider) - 1u, 1u)
#define piano_clk_SetMode(clkMode)               piano_clk_SetModeRegister(clkMode)
#define piano_clk_SetSource(clkSource)           piano_clk_SetSourceRegister(clkSource)
#if defined(piano_clk__CFG3)
#define piano_clk_SetPhase(clkPhase)             piano_clk_SetPhaseRegister(clkPhase)
#define piano_clk_SetPhaseValue(clkPhase)        piano_clk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(piano_clk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define piano_clk_CLKEN              (* (reg8 *) piano_clk__PM_ACT_CFG)
#define piano_clk_CLKEN_PTR          ((reg8 *) piano_clk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define piano_clk_CLKSTBY            (* (reg8 *) piano_clk__PM_STBY_CFG)
#define piano_clk_CLKSTBY_PTR        ((reg8 *) piano_clk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define piano_clk_DIV_LSB            (* (reg8 *) piano_clk__CFG0)
#define piano_clk_DIV_LSB_PTR        ((reg8 *) piano_clk__CFG0)
#define piano_clk_DIV_PTR            ((reg16 *) piano_clk__CFG0)

/* Clock MSB divider configuration register. */
#define piano_clk_DIV_MSB            (* (reg8 *) piano_clk__CFG1)
#define piano_clk_DIV_MSB_PTR        ((reg8 *) piano_clk__CFG1)

/* Mode and source configuration register */
#define piano_clk_MOD_SRC            (* (reg8 *) piano_clk__CFG2)
#define piano_clk_MOD_SRC_PTR        ((reg8 *) piano_clk__CFG2)

#if defined(piano_clk__CFG3)
/* Analog clock phase configuration register */
#define piano_clk_PHASE              (* (reg8 *) piano_clk__CFG3)
#define piano_clk_PHASE_PTR          ((reg8 *) piano_clk__CFG3)
#endif /* defined(piano_clk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define piano_clk_CLKEN_MASK         piano_clk__PM_ACT_MSK
#define piano_clk_CLKSTBY_MASK       piano_clk__PM_STBY_MSK

/* CFG2 field masks */
#define piano_clk_SRC_SEL_MSK        piano_clk__CFG2_SRC_SEL_MASK
#define piano_clk_MODE_MASK          (~(piano_clk_SRC_SEL_MSK))

#if defined(piano_clk__CFG3)
/* CFG3 phase mask */
#define piano_clk_PHASE_MASK         piano_clk__CFG3_PHASE_DLY_MASK
#endif /* defined(piano_clk__CFG3) */

#endif /* CY_CLOCK_piano_clk_H */


/* [] END OF FILE */
