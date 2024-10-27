/******************************************/
/************* SWD : SW ******************/
/************* Date : 18 Augest 2024 ******/
/************* Author : Mohamed Kaoud *****/
/************* Version : 1.0 **************/

#ifndef SSD_INTERFACE_H_
#define SSD_INTERFACE_H_

#define PULL_UP    0
#define PULL_DOWN  1

#define PRESSED    1
#define RELEASED   0

typedef struct
{
	u8 PortNumber;
	u8 PinNumber;
	u8 Type;
}SWITCH_t;

u8 SWITCH_u8IsPressed(SWITCH_t Switch);


#endif
