/*******************************************************************************
* File Name: PITCH_ADC_PM.c
* Version 3.0
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PITCH_ADC.h"


/***************************************
* Local data allocation
***************************************/

static PITCH_ADC_BACKUP_STRUCT  PITCH_ADC_backup =
{
    PITCH_ADC_DISABLED
};


/*******************************************************************************
* Function Name: PITCH_ADC_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void PITCH_ADC_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: PITCH_ADC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void PITCH_ADC_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: PITCH_ADC_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The PITCH_ADC_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PITCH_ADC_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void PITCH_ADC_Sleep(void)
{
    if((PITCH_ADC_PWRMGR_SAR_REG  & PITCH_ADC_ACT_PWR_SAR_EN) != 0u)
    {
        if((PITCH_ADC_SAR_CSR0_REG & PITCH_ADC_SAR_SOF_START_CONV) != 0u)
        {
            PITCH_ADC_backup.enableState = PITCH_ADC_ENABLED | PITCH_ADC_STARTED;
        }
        else
        {
            PITCH_ADC_backup.enableState = PITCH_ADC_ENABLED;
        }
        PITCH_ADC_Stop();
    }
    else
    {
        PITCH_ADC_backup.enableState = PITCH_ADC_DISABLED;
    }
}


/*******************************************************************************
* Function Name: PITCH_ADC_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  PITCH_ADC_Sleep() was called. If the component was enabled before the
*  PITCH_ADC_Sleep() function was called, the
*  PITCH_ADC_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PITCH_ADC_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void PITCH_ADC_Wakeup(void)
{
    if(PITCH_ADC_backup.enableState != PITCH_ADC_DISABLED)
    {
        PITCH_ADC_Enable();
        #if(PITCH_ADC_DEFAULT_CONV_MODE != PITCH_ADC__HARDWARE_TRIGGER)
            if((PITCH_ADC_backup.enableState & PITCH_ADC_STARTED) != 0u)
            {
                PITCH_ADC_StartConvert();
            }
        #endif /* End PITCH_ADC_DEFAULT_CONV_MODE != PITCH_ADC__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
