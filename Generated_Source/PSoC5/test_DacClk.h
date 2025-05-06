/*******************************************************************************
* File Name: test_DacClk.h
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

#if !defined(CY_CLOCK_test_DacClk_H)
#define CY_CLOCK_test_DacClk_H

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

void test_DacClk_Start(void) ;
void test_DacClk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void test_DacClk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void test_DacClk_StandbyPower(uint8 state) ;
void test_DacClk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 test_DacClk_GetDividerRegister(void) ;
void test_DacClk_SetModeRegister(uint8 modeBitMask) ;
void test_DacClk_ClearModeRegister(uint8 modeBitMask) ;
uint8 test_DacClk_GetModeRegister(void) ;
void test_DacClk_SetSourceRegister(uint8 clkSource) ;
uint8 test_DacClk_GetSourceRegister(void) ;
#if defined(test_DacClk__CFG3)
void test_DacClk_SetPhaseRegister(uint8 clkPhase) ;
uint8 test_DacClk_GetPhaseRegister(void) ;
#endif /* defined(test_DacClk__CFG3) */

#define test_DacClk_Enable()                       test_DacClk_Start()
#define test_DacClk_Disable()                      test_DacClk_Stop()
#define test_DacClk_SetDivider(clkDivider)         test_DacClk_SetDividerRegister(clkDivider, 1u)
#define test_DacClk_SetDividerValue(clkDivider)    test_DacClk_SetDividerRegister((clkDivider) - 1u, 1u)
#define test_DacClk_SetMode(clkMode)               test_DacClk_SetModeRegister(clkMode)
#define test_DacClk_SetSource(clkSource)           test_DacClk_SetSourceRegister(clkSource)
#if defined(test_DacClk__CFG3)
#define test_DacClk_SetPhase(clkPhase)             test_DacClk_SetPhaseRegister(clkPhase)
#define test_DacClk_SetPhaseValue(clkPhase)        test_DacClk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(test_DacClk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define test_DacClk_CLKEN              (* (reg8 *) test_DacClk__PM_ACT_CFG)
#define test_DacClk_CLKEN_PTR          ((reg8 *) test_DacClk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define test_DacClk_CLKSTBY            (* (reg8 *) test_DacClk__PM_STBY_CFG)
#define test_DacClk_CLKSTBY_PTR        ((reg8 *) test_DacClk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define test_DacClk_DIV_LSB            (* (reg8 *) test_DacClk__CFG0)
#define test_DacClk_DIV_LSB_PTR        ((reg8 *) test_DacClk__CFG0)
#define test_DacClk_DIV_PTR            ((reg16 *) test_DacClk__CFG0)

/* Clock MSB divider configuration register. */
#define test_DacClk_DIV_MSB            (* (reg8 *) test_DacClk__CFG1)
#define test_DacClk_DIV_MSB_PTR        ((reg8 *) test_DacClk__CFG1)

/* Mode and source configuration register */
#define test_DacClk_MOD_SRC            (* (reg8 *) test_DacClk__CFG2)
#define test_DacClk_MOD_SRC_PTR        ((reg8 *) test_DacClk__CFG2)

#if defined(test_DacClk__CFG3)
/* Analog clock phase configuration register */
#define test_DacClk_PHASE              (* (reg8 *) test_DacClk__CFG3)
#define test_DacClk_PHASE_PTR          ((reg8 *) test_DacClk__CFG3)
#endif /* defined(test_DacClk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define test_DacClk_CLKEN_MASK         test_DacClk__PM_ACT_MSK
#define test_DacClk_CLKSTBY_MASK       test_DacClk__PM_STBY_MSK

/* CFG2 field masks */
#define test_DacClk_SRC_SEL_MSK        test_DacClk__CFG2_SRC_SEL_MASK
#define test_DacClk_MODE_MASK          (~(test_DacClk_SRC_SEL_MSK))

#if defined(test_DacClk__CFG3)
/* CFG3 phase mask */
#define test_DacClk_PHASE_MASK         test_DacClk__CFG3_PHASE_DLY_MASK
#endif /* defined(test_DacClk__CFG3) */

#endif /* CY_CLOCK_test_DacClk_H */


/* [] END OF FILE */
