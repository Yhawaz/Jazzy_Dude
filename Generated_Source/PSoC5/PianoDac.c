/*******************************************************************************
* File Name: PianoDac.c  
* Version 1.90
*
* Description:
*  This file provides the source code to the API for the 8-bit Voltage DAC 
*  (VDAC8) User Module.
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "PianoDac.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 PianoDac_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 PianoDac_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static PianoDac_backupStruct PianoDac_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: PianoDac_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_Init(void) 
{
    PianoDac_CR0 = (PianoDac_MODE_V );

    /* Set default data source */
    #if(PianoDac_DEFAULT_DATA_SRC != 0 )
        PianoDac_CR1 = (PianoDac_DEFAULT_CNTL | PianoDac_DACBUS_ENABLE) ;
    #else
        PianoDac_CR1 = (PianoDac_DEFAULT_CNTL | PianoDac_DACBUS_DISABLE) ;
    #endif /* (PianoDac_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(PianoDac_DEFAULT_STRB != 0)
        PianoDac_Strobe |= PianoDac_STRB_EN ;
    #endif/* (PianoDac_DEFAULT_STRB != 0) */

    /* Set default range */
    PianoDac_SetRange(PianoDac_DEFAULT_RANGE); 

    /* Set default speed */
    PianoDac_SetSpeed(PianoDac_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: PianoDac_Enable
********************************************************************************
* Summary:
*  Enable the VDAC8
* 
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_Enable(void) 
{
    PianoDac_PWRMGR |= PianoDac_ACT_PWR_EN;
    PianoDac_STBY_PWRMGR |= PianoDac_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(PianoDac_restoreVal == 1u) 
        {
             PianoDac_CR0 = PianoDac_backup.data_value;
             PianoDac_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: PianoDac_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:
*  Power: Sets power level between off (0) and (3) high power
*
* Return:
*  void 
*
* Global variables:
*  PianoDac_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PianoDac_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(PianoDac_initVar == 0u)
    { 
        PianoDac_Init();
        PianoDac_initVar = 1u;
    }

    /* Enable power to DAC */
    PianoDac_Enable();

    /* Set default value */
    PianoDac_SetValue(PianoDac_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: PianoDac_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_Stop(void) 
{
    /* Disble power to DAC */
    PianoDac_PWRMGR &= (uint8)(~PianoDac_ACT_PWR_EN);
    PianoDac_STBY_PWRMGR &= (uint8)(~PianoDac_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        PianoDac_backup.data_value = PianoDac_CR0;
        PianoDac_CR0 = PianoDac_CUR_MODE_OUT_OFF;
        PianoDac_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: PianoDac_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    PianoDac_CR0 &= (uint8)(~PianoDac_HS_MASK);
    PianoDac_CR0 |=  (speed & PianoDac_HS_MASK);
}


/*******************************************************************************
* Function Name: PianoDac_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:
*  Range: Sets one of Three valid ranges.
*
* Return:
*  void 
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_SetRange(uint8 range) 
{
    PianoDac_CR0 &= (uint8)(~PianoDac_RANGE_MASK);      /* Clear existing mode */
    PianoDac_CR0 |= (range & PianoDac_RANGE_MASK);      /*  Set Range  */
    PianoDac_DacTrim();
}


/*******************************************************************************
* Function Name: PianoDac_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  void 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 PianoDac_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    PianoDac_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        PianoDac_Data = value;
        CyExitCriticalSection(PianoDac_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: PianoDac_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  range:  1V or 4V range.  See constants.
*
* Return:
*  void
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void PianoDac_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((PianoDac_CR0 & PianoDac_RANGE_MASK) >> 2) + PianoDac_TRIM_M7_1V_RNG_OFFSET;
    PianoDac_TR = CY_GET_XTND_REG8((uint8 *)(PianoDac_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
