/*******************************************************************************
 * Filename:			state_machine.h
 * Revised:				Date: 2020.02.20
 * Revision:			V001
 * Description:		    State machine function
*******************************************************************************/

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "common.h"

/*******************************************************************************
 * ENUMERATE
 ******************************************************************************/
// Machine status enumerate
typedef enum
{
	acceptCoinMachineStatus = 0,
	enoughCoinMachineStatus,
	dispensingMachineStatus,
	pauseDispenseMachineStatus,
	totalMachineStatus,
}
eMACHINE_STATUS;

/*******************************************************************************
 * STRUCTURE
 ******************************************************************************/
// Define state machine structure
typedef struct _sSTATE_MACHINE
{
	void (*Initialize)(void);
	void (*InsertCoin)(void);
	void (*DispenseButtonPressed)(void);
}
sSTATE_MACHINE;

/*******************************************************************************
 * PUBLIC VARIABLES
 ******************************************************************************/
extern sSTATE_MACHINE sStateMachine;

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _STATE_MACHINE_H_ */
