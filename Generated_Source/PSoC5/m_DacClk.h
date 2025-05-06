/*******************************************************************************
* File Name: m_DacClk.h
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

#if !defined(CY_CLOCK_m_DacClk_H)
#define CY_CLOCK_m_DacClk_H

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

void m_DacClk_Start(void) ;
void m_DacClk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void m_DacClk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void m_DacClk_StandbyPower(uint8 state) ;
void m_DacClk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 m_DacClk_GetDividerRegister(void) ;
void m_DacClk_SetModeRegister(uint8 modeBitMask) ;
void m_DacClk_ClearModeRegister(uint8 modeBitMask) ;
uint8 m_DacClk_GetModeRegister(void) ;
void m_DacClk_SetSourceRegister(uint8 clkSource) ;
uint8 m_DacClk_GetSourceRegister(void) ;
#if defined(m_DacClk__CFG3)
void m_DacClk_SetPhaseRegister(uint8 clkPhase) ;
uint8 m_DacClk_GetPhaseRegister(void) ;
#endif /* defined(m_DacClk__CFG3) */

#define m_DacClk_Enable()                       m_DacClk_Start()
#define m_DacClk_Disable()                      m_DacClk_Stop()
#define m_DacClk_SetDivider(clkDivider)         m_DacClk_SetDividerRegister(clkDivider, 1u)
#define m_DacClk_SetDividerValue(clkDivider)    m_DacClk_SetDividerRegister((clkDivider) - 1u, 1u)
#define m_DacClk_SetMode(clkMode)               m_DacClk_SetModeRegister(clkMode)
#define m_DacClk_SetSource(clkSource)           m_DacClk_SetSourceRegister(clkSource)
#if defined(m_DacClk__CFG3)
#define m_DacClk_SetPhase(clkPhase)             m_DacClk_SetPhaseRegister(clkPhase)
#define m_DacClk_SetPhaseValue(clkPhase)        m_DacClk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(m_DacClk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define m_DacClk_CLKEN              (* (reg8 *) m_DacClk__PM_ACT_CFG)
#define m_DacClk_CLKEN_PTR          ((reg8 *) m_DacClk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define m_DacClk_CLKSTBY            (* (reg8 *) m_DacClk__PM_STBY_CFG)
#define m_DacClk_CLKSTBY_PTR        ((reg8 *) m_DacClk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define m_DacClk_DIV_LSB            (* (reg8 *) m_DacClk__CFG0)
#define m_DacClk_DIV_LSB_PTR        ((reg8 *) m_DacClk__CFG0)
#define m_DacClk_DIV_PTR            ((reg16 *) m_DacClk__CFG0)

/* Clock MSB divider configuration register. */
#define m_DacClk_DIV_MSB            (* (reg8 *) m_DacClk__CFG1)
#define m_DacClk_DIV_MSB_PTR        ((reg8 *) m_DacClk__CFG1)

/* Mode and source configuration register */
#define m_DacClk_MOD_SRC            (* (reg8 *) m_DacClk__CFG2)
#define m_DacClk_MOD_SRC_PTR        ((reg8 *) m_DacClk__CFG2)

#if defined(m_DacClk__CFG3)
/* Analog clock phase configuration register */
#define m_DacClk_PHASE              (* (reg8 *) m_DacClk__CFG3)
#define m_DacClk_PHASE_PTR          ((reg8 *) m_DacClk__CFG3)
#endif /* defined(m_DacClk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define m_DacClk_CLKEN_MASK         m_DacClk__PM_ACT_MSK
#define m_DacClk_CLKSTBY_MASK       m_DacClk__PM_STBY_MSK

/* CFG2 field masks */
#define m_DacClk_SRC_SEL_MSK        m_DacClk__CFG2_SRC_SEL_MASK
#define m_DacClk_MODE_MASK          (~(m_DacClk_SRC_SEL_MSK))

#if defined(m_DacClk__CFG3)
/* CFG3 phase mask */
#define m_DacClk_PHASE_MASK         m_DacClk__CFG3_PHASE_DLY_MASK
#endif /* defined(m_DacClk__CFG3) */

#endif /* CY_CLOCK_m_DacClk_H */


/* [] END OF FILE */
