/*******************************************************************************
* File Name: SaxDac.c  
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
#include "SaxDac.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 SaxDac_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 SaxDac_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static SaxDac_backupStruct SaxDac_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: SaxDac_Init
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
void SaxDac_Init(void) 
{
    SaxDac_CR0 = (SaxDac_MODE_V );

    /* Set default data source */
    #if(SaxDac_DEFAULT_DATA_SRC != 0 )
        SaxDac_CR1 = (SaxDac_DEFAULT_CNTL | SaxDac_DACBUS_ENABLE) ;
    #else
        SaxDac_CR1 = (SaxDac_DEFAULT_CNTL | SaxDac_DACBUS_DISABLE) ;
    #endif /* (SaxDac_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(SaxDac_DEFAULT_STRB != 0)
        SaxDac_Strobe |= SaxDac_STRB_EN ;
    #endif/* (SaxDac_DEFAULT_STRB != 0) */

    /* Set default range */
    SaxDac_SetRange(SaxDac_DEFAULT_RANGE); 

    /* Set default speed */
    SaxDac_SetSpeed(SaxDac_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: SaxDac_Enable
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
void SaxDac_Enable(void) 
{
    SaxDac_PWRMGR |= SaxDac_ACT_PWR_EN;
    SaxDac_STBY_PWRMGR |= SaxDac_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(SaxDac_restoreVal == 1u) 
        {
             SaxDac_CR0 = SaxDac_backup.data_value;
             SaxDac_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: SaxDac_Start
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
*  SaxDac_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void SaxDac_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(SaxDac_initVar == 0u)
    { 
        SaxDac_Init();
        SaxDac_initVar = 1u;
    }

    /* Enable power to DAC */
    SaxDac_Enable();

    /* Set default value */
    SaxDac_SetValue(SaxDac_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: SaxDac_Stop
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
void SaxDac_Stop(void) 
{
    /* Disble power to DAC */
    SaxDac_PWRMGR &= (uint8)(~SaxDac_ACT_PWR_EN);
    SaxDac_STBY_PWRMGR &= (uint8)(~SaxDac_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        SaxDac_backup.data_value = SaxDac_CR0;
        SaxDac_CR0 = SaxDac_CUR_MODE_OUT_OFF;
        SaxDac_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: SaxDac_SetSpeed
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
void SaxDac_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    SaxDac_CR0 &= (uint8)(~SaxDac_HS_MASK);
    SaxDac_CR0 |=  (speed & SaxDac_HS_MASK);
}


/*******************************************************************************
* Function Name: SaxDac_SetRange
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
void SaxDac_SetRange(uint8 range) 
{
    SaxDac_CR0 &= (uint8)(~SaxDac_RANGE_MASK);      /* Clear existing mode */
    SaxDac_CR0 |= (range & SaxDac_RANGE_MASK);      /*  Set Range  */
    SaxDac_DacTrim();
}


/*******************************************************************************
* Function Name: SaxDac_SetValue
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
void SaxDac_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 SaxDac_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    SaxDac_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        SaxDac_Data = value;
        CyExitCriticalSection(SaxDac_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: SaxDac_DacTrim
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
void SaxDac_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((SaxDac_CR0 & SaxDac_RANGE_MASK) >> 2) + SaxDac_TRIM_M7_1V_RNG_OFFSET;
    SaxDac_TR = CY_GET_XTND_REG8((uint8 *)(SaxDac_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
