/******************************************/
/************* SWD : SW ******************/
/************* Date : 18 Augest 2024 ******/
/************* Author : Mohamed Kaoud *****/
/************* Version : 1.0 **************/

#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "SW_interface.h"
#include "SW_private.h"
#include "SW_config.h"

u8 SWITCH_u8IsPressed(SWITCH_t Switch)
{
	u8 IsPressed = 0;
	u8 PinValue = DIO_u8GetPinValue(Switch.PortNumber , Switch.PinNumber);
	if(PULL_UP == Switch.Type)
	{
		if(PinValue == RELEASED)
		{
			IsPressed = 1;
		}

	}
	else if(PULL_DOWN == Switch.Type)
	{
		if(PinValue == PRESSED)
		{
			IsPressed = 1;
		}
	}
	return IsPressed;
}
