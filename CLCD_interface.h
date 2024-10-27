#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_

//typedef struct
//{
//	u8 Port;
//	u8 Pin;
//	u8 Type;
//
//}LED_t;
//#define SOURCE 1
//#define  SINK 0
//#define ON 1
//#define OF 0



void CLCD_voidInit(void);
void CLCD_voidSendString(const char *PcString );
void CLCD_voidSitPosition(u8 Copy_u8x,u8 Copy_u8y );
void CLCD_voidSendSpecialChar(u8 *Pu8_arr,u8 Copy_u8BlockNum, u8 Copy_u8x,u8 Copy_u8y);
void CLCD_voidClearDisplay(void);
#endif
