/*******************************************************************************
* File Name: SaxDac_PM.c  
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

#include "SaxDac.h"

static SaxDac_backupStruct SaxDac_backup;


/*******************************************************************************
* Function Name: SaxDac_SaveConfig
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
void SaxDac_SaveConfig(void) 
{
    if (!((SaxDac_CR1 & SaxDac_SRC_MASK) == SaxDac_SRC_UDB))
    {
        SaxDac_backup.data_value = SaxDac_Data;
    }
}


/*******************************************************************************
* Function Name: SaxDac_RestoreConfig
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
void SaxDac_RestoreConfig(void) 
{
    if (!((SaxDac_CR1 & SaxDac_SRC_MASK) == SaxDac_SRC_UDB))
    {
        if((SaxDac_Strobe & SaxDac_STRB_MASK) == SaxDac_STRB_EN)
        {
            SaxDac_Strobe &= (uint8)(~SaxDac_STRB_MASK);
            SaxDac_Data = SaxDac_backup.data_value;
            SaxDac_Strobe |= SaxDac_STRB_EN;
        }
        else
        {
            SaxDac_Data = SaxDac_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: SaxDac_Sleep
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
*  SaxDac_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void SaxDac_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(SaxDac_ACT_PWR_EN == (SaxDac_PWRMGR & SaxDac_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        SaxDac_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        SaxDac_backup.enableState = 0u;
    }
    
    SaxDac_Stop();
    SaxDac_SaveConfig();
}


/*******************************************************************************
* Function Name: SaxDac_Wakeup
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
*  SaxDac_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SaxDac_Wakeup(void) 
{
    SaxDac_RestoreConfig();
    
    if(SaxDac_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        SaxDac_Enable();

        /* Restore the data register */
        SaxDac_SetValue(SaxDac_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
