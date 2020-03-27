/*******************************************************************************
 * Filename:			main_loop.c
 * Revised:				Date: 2020.02.20
 * Revision:			V001
 * Description:		    Main loop function
*******************************************************************************/

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "main_loop.h"
#include "software_timer.h"
#include "state_machine.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/
#define DEBOUNCE_DELAY	50

/*******************************************************************************
 * PUBLIC VARIABLES
 ******************************************************************************/
volatile uint32_t eventFlags = 0;

/*******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************/
uint8_t debounceTimerId[2];

/*******************************************************************************
 * @fn      DebounceTimerCallback
 * @brief   Debounce timer callback
 * @paramz  softwareTimerId
 * @return  None
 ******************************************************************************/
void DebounceTimerCallback(uint8_t softwareTimerId)
{
	eventFlags |= (0x01 << softwareTimerId);
}

/*******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * EVENT FLAG FUNCTIONS
 ******************************************************************************/
static void InsertCoinEventFlag(void);
static void ButtonPressedEventFlag(void);

// Initial event flag jump table
static void (*EventFlags[])(void) =
{
	InsertCoinEventFlag,
	ButtonPressedEventFlag,
};

/*******************************************************************************
 * @fn      InsertCoinEventFlag
 * @brief   Insert coin event flag
 * @paramz  None
 * @return  None
 ******************************************************************************/
static void InsertCoinEventFlag(void)
{
	if(HAL_GPIO_ReadPin(INSERT_COIN_GPIO_Port, INSERT_COIN_Pin) == GPIO_PIN_RESET)
	{
		sStateMachine.InsertCoin();
	}
}

/*******************************************************************************
 * @fn      ButtonPressedEventFlag
 * @brief   Button pressed event flag
 * @paramz  None
 * @return  None
 ******************************************************************************/
static void ButtonPressedEventFlag(void)
{
	if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)
	{
		sStateMachine.DispenseButtonPressed();
	}
}

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * @fn		MainLoop
 * @brief	Main loop
 * @param	None
 ******************************************************************************/
// https://blog.csdn.net/liangsir_l/article/details/50707864
void MainLoop(void)
{
    uint8_t i = 0;

    // Enable software timer
    sSoftwareTimer.Enable();

    for(i = 0; i < 2; i++)
    {
  	  debounceTimerId[i] = sSoftwareTimer.Initialize(NULL, DebounceTimerCallback, NULL, TIMER_ONCE_TYPE);
    }

    sStateMachine.Initialize();

    for(;;)
    {
        if(eventFlags != 0)
        {
            for(i = 0; i < maximumEventFlag; i++)
            {
            	// Detected event flag
                if(((eventFlags >> i) & 0x01) == 0x01)
                {
                	// Call related function through jump table
                    (*EventFlags[i])();
                    // Clear event flag
                    eventFlags &= (~(0x01 << i));
                }
            }
        }
    }
}

/*******************************************************************************
 * INTERRUPT CALLBACK
 ******************************************************************************/
/*******************************************************************************
 * @fn      HAL_GPIO_EXTI_Callback
 * @brief   EXTI line detection callback
 * @param	gpioPin
 * @return	None
 ******************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t gpioPin)
{
	switch(gpioPin)
	{
		case INSERT_COIN_Pin:
			sSoftwareTimer.Start(debounceTimerId[0], DEBOUNCE_DELAY);
			break;
		case BUTTON_Pin:
			sSoftwareTimer.Start(debounceTimerId[1], DEBOUNCE_DELAY);
			break;
		default:
			break;
	}
}
