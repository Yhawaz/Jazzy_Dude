/*******************************************************************************
* File Name: mm_VDAC8_PM.c  
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

#include "mm_VDAC8.h"

static mm_VDAC8_backupStruct mm_VDAC8_backup;


/*******************************************************************************
* Function Name: mm_VDAC8_SaveConfig
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
void mm_VDAC8_SaveConfig(void) 
{
    if (!((mm_VDAC8_CR1 & mm_VDAC8_SRC_MASK) == mm_VDAC8_SRC_UDB))
    {
        mm_VDAC8_backup.data_value = mm_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: mm_VDAC8_RestoreConfig
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
void mm_VDAC8_RestoreConfig(void) 
{
    if (!((mm_VDAC8_CR1 & mm_VDAC8_SRC_MASK) == mm_VDAC8_SRC_UDB))
    {
        if((mm_VDAC8_Strobe & mm_VDAC8_STRB_MASK) == mm_VDAC8_STRB_EN)
        {
            mm_VDAC8_Strobe &= (uint8)(~mm_VDAC8_STRB_MASK);
            mm_VDAC8_Data = mm_VDAC8_backup.data_value;
            mm_VDAC8_Strobe |= mm_VDAC8_STRB_EN;
        }
        else
        {
            mm_VDAC8_Data = mm_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: mm_VDAC8_Sleep
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
*  mm_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void mm_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(mm_VDAC8_ACT_PWR_EN == (mm_VDAC8_PWRMGR & mm_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        mm_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        mm_VDAC8_backup.enableState = 0u;
    }
    
    mm_VDAC8_Stop();
    mm_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: mm_VDAC8_Wakeup
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
*  mm_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void mm_VDAC8_Wakeup(void) 
{
    mm_VDAC8_RestoreConfig();
    
    if(mm_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        mm_VDAC8_Enable();

        /* Restore the data register */
        mm_VDAC8_SetValue(mm_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
