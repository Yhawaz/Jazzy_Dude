/*******************************************************************************
* File Name: test_PM.c  
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

#include "test.h"

static test_BACKUP_STRUCT  test_backup;


/*******************************************************************************
* Function Name: test_Sleep
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
*  test_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void test_Sleep(void) 
{
	/* Save DAC8's enable state */

	test_backup.enableState = (test_VDAC8_ACT_PWR_EN == 
		(test_VDAC8_PWRMGR_REG & test_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	test_Stop();
	test_SaveConfig();
}


/*******************************************************************************
* Function Name: test_Wakeup
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
*  test_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void test_Wakeup(void) 
{
	test_RestoreConfig();

	if(test_backup.enableState == 1u)
	{
		test_Enable();
	}
}


/* [] END OF FILE */
