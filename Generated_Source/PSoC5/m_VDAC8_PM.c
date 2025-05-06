/*******************************************************************************
* File Name: m_VDAC8_PM.c  
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

#include "m_VDAC8.h"

static m_VDAC8_backupStruct m_VDAC8_backup;


/*******************************************************************************
* Function Name: m_VDAC8_SaveConfig
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
void m_VDAC8_SaveConfig(void) 
{
    if (!((m_VDAC8_CR1 & m_VDAC8_SRC_MASK) == m_VDAC8_SRC_UDB))
    {
        m_VDAC8_backup.data_value = m_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: m_VDAC8_RestoreConfig
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
void m_VDAC8_RestoreConfig(void) 
{
    if (!((m_VDAC8_CR1 & m_VDAC8_SRC_MASK) == m_VDAC8_SRC_UDB))
    {
        if((m_VDAC8_Strobe & m_VDAC8_STRB_MASK) == m_VDAC8_STRB_EN)
        {
            m_VDAC8_Strobe &= (uint8)(~m_VDAC8_STRB_MASK);
            m_VDAC8_Data = m_VDAC8_backup.data_value;
            m_VDAC8_Strobe |= m_VDAC8_STRB_EN;
        }
        else
        {
            m_VDAC8_Data = m_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: m_VDAC8_Sleep
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
*  m_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void m_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(m_VDAC8_ACT_PWR_EN == (m_VDAC8_PWRMGR & m_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        m_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        m_VDAC8_backup.enableState = 0u;
    }
    
    m_VDAC8_Stop();
    m_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: m_VDAC8_Wakeup
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
*  m_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void m_VDAC8_Wakeup(void) 
{
    m_VDAC8_RestoreConfig();
    
    if(m_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        m_VDAC8_Enable();

        /* Restore the data register */
        m_VDAC8_SetValue(m_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
