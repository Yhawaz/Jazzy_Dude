/*******************************************************************************
* File Name: test_VDAC8_PM.c  
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

#include "test_VDAC8.h"

static test_VDAC8_backupStruct test_VDAC8_backup;


/*******************************************************************************
* Function Name: test_VDAC8_SaveConfig
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
void test_VDAC8_SaveConfig(void) 
{
    if (!((test_VDAC8_CR1 & test_VDAC8_SRC_MASK) == test_VDAC8_SRC_UDB))
    {
        test_VDAC8_backup.data_value = test_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: test_VDAC8_RestoreConfig
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
void test_VDAC8_RestoreConfig(void) 
{
    if (!((test_VDAC8_CR1 & test_VDAC8_SRC_MASK) == test_VDAC8_SRC_UDB))
    {
        if((test_VDAC8_Strobe & test_VDAC8_STRB_MASK) == test_VDAC8_STRB_EN)
        {
            test_VDAC8_Strobe &= (uint8)(~test_VDAC8_STRB_MASK);
            test_VDAC8_Data = test_VDAC8_backup.data_value;
            test_VDAC8_Strobe |= test_VDAC8_STRB_EN;
        }
        else
        {
            test_VDAC8_Data = test_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: test_VDAC8_Sleep
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
*  test_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void test_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(test_VDAC8_ACT_PWR_EN == (test_VDAC8_PWRMGR & test_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        test_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        test_VDAC8_backup.enableState = 0u;
    }
    
    test_VDAC8_Stop();
    test_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: test_VDAC8_Wakeup
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
*  test_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void test_VDAC8_Wakeup(void) 
{
    test_VDAC8_RestoreConfig();
    
    if(test_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        test_VDAC8_Enable();

        /* Restore the data register */
        test_VDAC8_SetValue(test_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
