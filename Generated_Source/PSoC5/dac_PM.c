/*******************************************************************************
* File Name: dac_PM.c  
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

#include "dac.h"

static dac_BACKUP_STRUCT  dac_backup;


/*******************************************************************************
* Function Name: dac_Sleep
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
*  dac_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void dac_Sleep(void) 
{
	/* Save DAC8's enable state */

	dac_backup.enableState = (dac_VDAC8_ACT_PWR_EN == 
		(dac_VDAC8_PWRMGR_REG & dac_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	dac_Stop();
	dac_SaveConfig();
}


/*******************************************************************************
* Function Name: dac_Wakeup
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
*  dac_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void dac_Wakeup(void) 
{
	dac_RestoreConfig();

	if(dac_backup.enableState == 1u)
	{
		dac_Enable();
	}
}


/* [] END OF FILE */
