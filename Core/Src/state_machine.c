/*******************************************************************************
 * Filename:			state_machine.c
 * Revised:				Date: 2020.02.20
 * Revision:			V001
 * Description:		    State machine function
*******************************************************************************/

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "state_machine.h"
#include "software_timer.h"

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/
#define MINIMUM_COINS	5

/*******************************************************************************
 * LOCAL VARIBLES
 ******************************************************************************/
/*******************************************************************************
 * STRUCTURE
 ******************************************************************************/
// Define state machine property structure
typedef struct
{
	eMACHINE_STATUS eCurrentMachineStatus;
	uint8_t totalCoin;
	uint8_t dispensingTimerId;
}
sSTATE_MACHINE_PRO;
static sSTATE_MACHINE_PRO sStateMachinePro;

/*******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************/

static void GoToInsertCoinMachineStatus(void);
static void GoToEnoughCoinMachineStatus(void);
static void GoToDispensingMachineStatus(void);
static void GoToPauseDispenseMachineStatus(void);
static void (*GoTo[])(void) =
{
	GoToInsertCoinMachineStatus,
	GoToEnoughCoinMachineStatus,
	GoToDispensingMachineStatus,
	GoToPauseDispenseMachineStatus,
};

/*******************************************************************************
 * @fn      GoToInsertCoinMachineStatus
 * @brief   Go to insert coin machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void GoToInsertCoinMachineStatus(void)
{
	sStateMachinePro.eCurrentMachineStatus = acceptCoinMachineStatus;
	sStateMachinePro.totalCoin = 0;
	printf("Accept coin status setup completed\n");
	printf("Please insert coin\n");
}

/*******************************************************************************
 * @fn      GoToEnoughCoinMachineStatus
 * @brief   Go to enough coin machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void GoToEnoughCoinMachineStatus(void)
{
	sStateMachinePro.eCurrentMachineStatus = enoughCoinMachineStatus;
	printf("Enough coin status setup completed\n");
	printf("Press button to dispense\n");
}

/*******************************************************************************
 * @fn      GoToDispensingMachineStatus
 * @brief   Go to dispensing machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void GoToDispensingMachineStatus(void)
{
	sStateMachinePro.eCurrentMachineStatus = dispensingMachineStatus;
	sSoftwareTimer.Start(sStateMachinePro.dispensingTimerId, 1000);
	printf("Dispensing status setup completed\n");
	printf("Press button to stop dispense\n");
}

/*******************************************************************************
 * @fn      GoToPauseDispenseMachineStatus
 * @brief   Go to pause dispense machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void GoToPauseDispenseMachineStatus(void)
{
	sStateMachinePro.eCurrentMachineStatus = pauseDispenseMachineStatus;
	sSoftwareTimer.Stop(sStateMachinePro.dispensingTimerId);
	printf("Pause Dispense status setup completed\n");
	printf("Press button to continue dispense\n");
}

static void DispensingTimerCallback(uint8_t softwareTimerId);

/*******************************************************************************
 * @fn      DispensingTimerCallback
 * @brief   Dispensing timer callback
 * @param   None
 * @return  None
 ******************************************************************************/
static void DispensingTimerCallback(uint8_t softwareTimerId)
{
	sStateMachinePro.totalCoin--;
	// Continue dispense
	if(sStateMachinePro.totalCoin > 0)
	{
		printf("Still remain %d second\n", sStateMachinePro.totalCoin);
		sSoftwareTimer.Start(sStateMachinePro.dispensingTimerId, 1000);
	}
	// Dispense complete
	else
	{
		(*GoTo[acceptCoinMachineStatus])();
	}
}

static void InsertCoinAtAcceptCoinMachineStatus(void);
static void InsertCoinAtEnoughCoinMachineStatus(void);
static void (*InsertCoinAction[])(void) =
{
	InsertCoinAtAcceptCoinMachineStatus,
	InsertCoinAtEnoughCoinMachineStatus,
};

/*******************************************************************************
 * @fn      InsertCoinAtAcceptCoinMachineStatus
 * @brief   Insert coin at accept coin machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void InsertCoinAtAcceptCoinMachineStatus(void)
{
	printf("Insert coin at accept coin machine status\n");
	sStateMachinePro.totalCoin++;
	printf("Total coin = %d\n", sStateMachinePro.totalCoin);
	if(sStateMachinePro.totalCoin >= MINIMUM_COINS)
	{
		(*GoTo[enoughCoinMachineStatus])();
	}
}

/*******************************************************************************
 * @fn      InsertCoinAtEnoughCoinMachineStatus
 * @brief   Insert coin at enough coin machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void InsertCoinAtEnoughCoinMachineStatus(void)
{
	printf("Insert coin at enough coin machine status\n");
	sStateMachinePro.totalCoin++;
	printf("Total coin = %d\n", sStateMachinePro.totalCoin);
}

static void DispenseButtonPressedAtEnoughCoinMachineStatus(void);
static void DispenseButtonPressedAtDispensingMachineStatus(void);
static void DispenseButtonPressedAtPauseDispenseMachineStatus(void);
static void (*DispenseButtonPressedAction[])(void) =
{
	NULL,
	DispenseButtonPressedAtEnoughCoinMachineStatus,
	DispenseButtonPressedAtDispensingMachineStatus,
	DispenseButtonPressedAtPauseDispenseMachineStatus
};

/*******************************************************************************
 * @fn      DispenseButtonPressedAtEnoughCoinMachineStatus
 * @brief   Dispense button pressed at enough coin machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void DispenseButtonPressedAtEnoughCoinMachineStatus(void)
{
	printf("Press button at enough coin machine status\n");
	(*GoTo[dispensingMachineStatus])();
}

/*******************************************************************************
 * @fn      DispenseButtonPressedAtDispensingMachineStatus
 * @brief   Dispense button pressed at dispensing machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void DispenseButtonPressedAtDispensingMachineStatus(void)
{
	printf("Press button at dispensing machine status\n");
	(*GoTo[pauseDispenseMachineStatus])();
}

/*******************************************************************************
 * @fn      DispenseButtonPressedAtPauseDispenseMachineStatus
 * @brief   Dispense button pressed at pause dispense machine status
 * @param   None
 * @return  None
 ******************************************************************************/
static void DispenseButtonPressedAtPauseDispenseMachineStatus(void)
{
	printf("Press button at pause dispense machine status\n");
	(*GoTo[dispensingMachineStatus])();
}

static void Initialize(void);
static void InsertCoin(void);
static void DispenseButtonPressed(void);

/*******************************************************************************
 * @fn      Initialize
 * @brief   Initial user state machine
 * @param   None
 * @return  None
 ******************************************************************************/
static void Initialize(void)
{
	(*GoTo[acceptCoinMachineStatus])();
	sStateMachinePro.dispensingTimerId = sSoftwareTimer.Initialize(NULL, DispensingTimerCallback, NULL, TIMER_ONCE_TYPE);
}

/*******************************************************************************
 * @fn      InsertCoin
 * @brief   Insert coin
 * @param   pulseValue
 * @return  None
 ******************************************************************************/
static void InsertCoin(void)
{
	if((sizeof(InsertCoinAction) / 4) > sStateMachinePro.eCurrentMachineStatus &&
		InsertCoinAction[sStateMachinePro.eCurrentMachineStatus] != NULL)
	{
		(*InsertCoinAction[sStateMachinePro.eCurrentMachineStatus])();
	}
	else
	{
		printf("Cannot insert coin at this status\n");
	}
}

/*******************************************************************************
 * @fn      DispenseButtonPressed
 * @brief   Dispense button pressed
 * @param   eButton
 * @return  None
 ******************************************************************************/
static void DispenseButtonPressed(void)
{
	if((sizeof(DispenseButtonPressedAction) / 4) > sStateMachinePro.eCurrentMachineStatus &&
		DispenseButtonPressedAction[sStateMachinePro.eCurrentMachineStatus] != NULL)
	{
		(*DispenseButtonPressedAction[sStateMachinePro.eCurrentMachineStatus])();
	}
	else
	{
		printf("Cannot press button at this status\n");
	}
}

// State machine
sSTATE_MACHINE sStateMachine =
{
	Initialize,
    InsertCoin,
    DispenseButtonPressed,
};
