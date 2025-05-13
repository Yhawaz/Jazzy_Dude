/*******************************************************************************
* File Name: Inst_Switch.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Inst_Switch_H) /* Pins Inst_Switch_H */
#define CY_PINS_Inst_Switch_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Inst_Switch_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Inst_Switch__PORT == 15 && ((Inst_Switch__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Inst_Switch_Write(uint8 value);
void    Inst_Switch_SetDriveMode(uint8 mode);
uint8   Inst_Switch_ReadDataReg(void);
uint8   Inst_Switch_Read(void);
void    Inst_Switch_SetInterruptMode(uint16 position, uint16 mode);
uint8   Inst_Switch_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Inst_Switch_SetDriveMode() function.
     *  @{
     */
        #define Inst_Switch_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Inst_Switch_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Inst_Switch_DM_RES_UP          PIN_DM_RES_UP
        #define Inst_Switch_DM_RES_DWN         PIN_DM_RES_DWN
        #define Inst_Switch_DM_OD_LO           PIN_DM_OD_LO
        #define Inst_Switch_DM_OD_HI           PIN_DM_OD_HI
        #define Inst_Switch_DM_STRONG          PIN_DM_STRONG
        #define Inst_Switch_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Inst_Switch_MASK               Inst_Switch__MASK
#define Inst_Switch_SHIFT              Inst_Switch__SHIFT
#define Inst_Switch_WIDTH              1u

/* Interrupt constants */
#if defined(Inst_Switch__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Inst_Switch_SetInterruptMode() function.
     *  @{
     */
        #define Inst_Switch_INTR_NONE      (uint16)(0x0000u)
        #define Inst_Switch_INTR_RISING    (uint16)(0x0001u)
        #define Inst_Switch_INTR_FALLING   (uint16)(0x0002u)
        #define Inst_Switch_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Inst_Switch_INTR_MASK      (0x01u) 
#endif /* (Inst_Switch__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Inst_Switch_PS                     (* (reg8 *) Inst_Switch__PS)
/* Data Register */
#define Inst_Switch_DR                     (* (reg8 *) Inst_Switch__DR)
/* Port Number */
#define Inst_Switch_PRT_NUM                (* (reg8 *) Inst_Switch__PRT) 
/* Connect to Analog Globals */                                                  
#define Inst_Switch_AG                     (* (reg8 *) Inst_Switch__AG)                       
/* Analog MUX bux enable */
#define Inst_Switch_AMUX                   (* (reg8 *) Inst_Switch__AMUX) 
/* Bidirectional Enable */                                                        
#define Inst_Switch_BIE                    (* (reg8 *) Inst_Switch__BIE)
/* Bit-mask for Aliased Register Access */
#define Inst_Switch_BIT_MASK               (* (reg8 *) Inst_Switch__BIT_MASK)
/* Bypass Enable */
#define Inst_Switch_BYP                    (* (reg8 *) Inst_Switch__BYP)
/* Port wide control signals */                                                   
#define Inst_Switch_CTL                    (* (reg8 *) Inst_Switch__CTL)
/* Drive Modes */
#define Inst_Switch_DM0                    (* (reg8 *) Inst_Switch__DM0) 
#define Inst_Switch_DM1                    (* (reg8 *) Inst_Switch__DM1)
#define Inst_Switch_DM2                    (* (reg8 *) Inst_Switch__DM2) 
/* Input Buffer Disable Override */
#define Inst_Switch_INP_DIS                (* (reg8 *) Inst_Switch__INP_DIS)
/* LCD Common or Segment Drive */
#define Inst_Switch_LCD_COM_SEG            (* (reg8 *) Inst_Switch__LCD_COM_SEG)
/* Enable Segment LCD */
#define Inst_Switch_LCD_EN                 (* (reg8 *) Inst_Switch__LCD_EN)
/* Slew Rate Control */
#define Inst_Switch_SLW                    (* (reg8 *) Inst_Switch__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Inst_Switch_PRTDSI__CAPS_SEL       (* (reg8 *) Inst_Switch__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Inst_Switch_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Inst_Switch__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Inst_Switch_PRTDSI__OE_SEL0        (* (reg8 *) Inst_Switch__PRTDSI__OE_SEL0) 
#define Inst_Switch_PRTDSI__OE_SEL1        (* (reg8 *) Inst_Switch__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Inst_Switch_PRTDSI__OUT_SEL0       (* (reg8 *) Inst_Switch__PRTDSI__OUT_SEL0) 
#define Inst_Switch_PRTDSI__OUT_SEL1       (* (reg8 *) Inst_Switch__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Inst_Switch_PRTDSI__SYNC_OUT       (* (reg8 *) Inst_Switch__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Inst_Switch__SIO_CFG)
    #define Inst_Switch_SIO_HYST_EN        (* (reg8 *) Inst_Switch__SIO_HYST_EN)
    #define Inst_Switch_SIO_REG_HIFREQ     (* (reg8 *) Inst_Switch__SIO_REG_HIFREQ)
    #define Inst_Switch_SIO_CFG            (* (reg8 *) Inst_Switch__SIO_CFG)
    #define Inst_Switch_SIO_DIFF           (* (reg8 *) Inst_Switch__SIO_DIFF)
#endif /* (Inst_Switch__SIO_CFG) */

/* Interrupt Registers */
#if defined(Inst_Switch__INTSTAT)
    #define Inst_Switch_INTSTAT            (* (reg8 *) Inst_Switch__INTSTAT)
    #define Inst_Switch_SNAP               (* (reg8 *) Inst_Switch__SNAP)
    
	#define Inst_Switch_0_INTTYPE_REG 		(* (reg8 *) Inst_Switch__0__INTTYPE)
#endif /* (Inst_Switch__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Inst_Switch_H */


/* [] END OF FILE */
