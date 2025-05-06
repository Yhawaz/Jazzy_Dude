/*******************************************************************************
* File Name: m_PM.c  
* Version 2.10
*
* Description:
*  This file provides the power manager source code to the API for 
*  the WaveDAC8 component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "m.h"

static m_BACKUP_STRUCT  m_backup;


/*******************************************************************************
* Function Name: m_Sleep
********************************************************************************
*
* Summary:
*  Stops the component and saves its configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  m_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void m_Sleep(void) 
{
	/* Save DAC8's enable state */

	m_backup.enableState = (m_VDAC8_ACT_PWR_EN == 
		(m_VDAC8_PWRMGR_REG & m_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	m_Stop();
	m_SaveConfig();
}


/*******************************************************************************
* Function Name: m_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component configuration. Should be called
*  just after awaking from sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  void
*
* Global variables:
*  m_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void m_Wakeup(void) 
{
	m_RestoreConfig();

	if(m_backup.enableState == 1u)
	{
		m_Enable();
	}
}


/* [] END OF FILE */
