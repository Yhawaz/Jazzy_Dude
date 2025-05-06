/*******************************************************************************
* File Name: Sax_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Sax_Timer.h"

static Sax_Timer_backupStruct Sax_Timer_backup;


/*******************************************************************************
* Function Name: Sax_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sax_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Sax_Timer_SaveConfig(void) 
{
    #if (!Sax_Timer_UsingFixedFunction)
        Sax_Timer_backup.TimerUdb = Sax_Timer_ReadCounter();
        Sax_Timer_backup.InterruptMaskValue = Sax_Timer_STATUS_MASK;
        #if (Sax_Timer_UsingHWCaptureCounter)
            Sax_Timer_backup.TimerCaptureCounter = Sax_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Sax_Timer_UDB_CONTROL_REG_REMOVED)
            Sax_Timer_backup.TimerControlRegister = Sax_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Sax_Timer_RestoreConfig
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
* Global variables:
*  Sax_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Sax_Timer_RestoreConfig(void) 
{   
    #if (!Sax_Timer_UsingFixedFunction)

        Sax_Timer_WriteCounter(Sax_Timer_backup.TimerUdb);
        Sax_Timer_STATUS_MASK =Sax_Timer_backup.InterruptMaskValue;
        #if (Sax_Timer_UsingHWCaptureCounter)
            Sax_Timer_SetCaptureCount(Sax_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Sax_Timer_UDB_CONTROL_REG_REMOVED)
            Sax_Timer_WriteControlRegister(Sax_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Sax_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sax_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Sax_Timer_Sleep(void) 
{
    #if(!Sax_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Sax_Timer_CTRL_ENABLE == (Sax_Timer_CONTROL & Sax_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Sax_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Sax_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Sax_Timer_Stop();
    Sax_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Sax_Timer_Wakeup
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
*  Sax_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Sax_Timer_Wakeup(void) 
{
    Sax_Timer_RestoreConfig();
    #if(!Sax_Timer_UDB_CONTROL_REG_REMOVED)
        if(Sax_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Sax_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
