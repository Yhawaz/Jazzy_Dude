/*******************************************************************************
* File Name: dac_VDAC8_PM.c  
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

#include "dac_VDAC8.h"

static dac_VDAC8_backupStruct dac_VDAC8_backup;


/*******************************************************************************
* Function Name: dac_VDAC8_SaveConfig
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
void dac_VDAC8_SaveConfig(void) 
{
    if (!((dac_VDAC8_CR1 & dac_VDAC8_SRC_MASK) == dac_VDAC8_SRC_UDB))
    {
        dac_VDAC8_backup.data_value = dac_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: dac_VDAC8_RestoreConfig
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
void dac_VDAC8_RestoreConfig(void) 
{
    if (!((dac_VDAC8_CR1 & dac_VDAC8_SRC_MASK) == dac_VDAC8_SRC_UDB))
    {
        if((dac_VDAC8_Strobe & dac_VDAC8_STRB_MASK) == dac_VDAC8_STRB_EN)
        {
            dac_VDAC8_Strobe &= (uint8)(~dac_VDAC8_STRB_MASK);
            dac_VDAC8_Data = dac_VDAC8_backup.data_value;
            dac_VDAC8_Strobe |= dac_VDAC8_STRB_EN;
        }
        else
        {
            dac_VDAC8_Data = dac_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: dac_VDAC8_Sleep
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
*  dac_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void dac_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(dac_VDAC8_ACT_PWR_EN == (dac_VDAC8_PWRMGR & dac_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        dac_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        dac_VDAC8_backup.enableState = 0u;
    }
    
    dac_VDAC8_Stop();
    dac_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: dac_VDAC8_Wakeup
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
*  dac_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void dac_VDAC8_Wakeup(void) 
{
    dac_VDAC8_RestoreConfig();
    
    if(dac_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        dac_VDAC8_Enable();

        /* Restore the data register */
        dac_VDAC8_SetValue(dac_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
