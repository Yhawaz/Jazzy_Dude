/*******************************************************************************
* File Name: PianoDac_PM.c  
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

#include "PianoDac.h"

static PianoDac_backupStruct PianoDac_backup;


/*******************************************************************************
* Function Name: PianoDac_SaveConfig
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
void PianoDac_SaveConfig(void) 
{
    if (!((PianoDac_CR1 & PianoDac_SRC_MASK) == PianoDac_SRC_UDB))
    {
        PianoDac_backup.data_value = PianoDac_Data;
    }
}


/*******************************************************************************
* Function Name: PianoDac_RestoreConfig
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
void PianoDac_RestoreConfig(void) 
{
    if (!((PianoDac_CR1 & PianoDac_SRC_MASK) == PianoDac_SRC_UDB))
    {
        if((PianoDac_Strobe & PianoDac_STRB_MASK) == PianoDac_STRB_EN)
        {
            PianoDac_Strobe &= (uint8)(~PianoDac_STRB_MASK);
            PianoDac_Data = PianoDac_backup.data_value;
            PianoDac_Strobe |= PianoDac_STRB_EN;
        }
        else
        {
            PianoDac_Data = PianoDac_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: PianoDac_Sleep
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
*  PianoDac_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void PianoDac_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(PianoDac_ACT_PWR_EN == (PianoDac_PWRMGR & PianoDac_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        PianoDac_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        PianoDac_backup.enableState = 0u;
    }
    
    PianoDac_Stop();
    PianoDac_SaveConfig();
}


/*******************************************************************************
* Function Name: PianoDac_Wakeup
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
*  PianoDac_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PianoDac_Wakeup(void) 
{
    PianoDac_RestoreConfig();
    
    if(PianoDac_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        PianoDac_Enable();

        /* Restore the data register */
        PianoDac_SetValue(PianoDac_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
