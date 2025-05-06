/*******************************************************************************
* File Name: SaxDac_VDAC8_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "SaxDac_VDAC8.h"

static SaxDac_VDAC8_backupStruct SaxDac_VDAC8_backup;


/*******************************************************************************
* Function Name: SaxDac_VDAC8_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void SaxDac_VDAC8_SaveConfig(void) 
{
    if (!((SaxDac_VDAC8_CR1 & SaxDac_VDAC8_SRC_MASK) == SaxDac_VDAC8_SRC_UDB))
    {
        SaxDac_VDAC8_backup.data_value = SaxDac_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: SaxDac_VDAC8_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void SaxDac_VDAC8_RestoreConfig(void) 
{
    if (!((SaxDac_VDAC8_CR1 & SaxDac_VDAC8_SRC_MASK) == SaxDac_VDAC8_SRC_UDB))
    {
        if((SaxDac_VDAC8_Strobe & SaxDac_VDAC8_STRB_MASK) == SaxDac_VDAC8_STRB_EN)
        {
            SaxDac_VDAC8_Strobe &= (uint8)(~SaxDac_VDAC8_STRB_MASK);
            SaxDac_VDAC8_Data = SaxDac_VDAC8_backup.data_value;
            SaxDac_VDAC8_Strobe |= SaxDac_VDAC8_STRB_EN;
        }
        else
        {
            SaxDac_VDAC8_Data = SaxDac_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: SaxDac_VDAC8_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  SaxDac_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void SaxDac_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(SaxDac_VDAC8_ACT_PWR_EN == (SaxDac_VDAC8_PWRMGR & SaxDac_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        SaxDac_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        SaxDac_VDAC8_backup.enableState = 0u;
    }
    
    SaxDac_VDAC8_Stop();
    SaxDac_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: SaxDac_VDAC8_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  SaxDac_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SaxDac_VDAC8_Wakeup(void) 
{
    SaxDac_VDAC8_RestoreConfig();
    
    if(SaxDac_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        SaxDac_VDAC8_Enable();

        /* Restore the data register */
        SaxDac_VDAC8_SetValue(SaxDac_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
