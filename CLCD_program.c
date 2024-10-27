#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "CLCD_private.h"
#include "CLCD_interface.h"
#include "CLCD_config.h"

#include <util/delay.h>


static void CLCD_voidSendData(u8 Copy_u8Data){
	DIO_voidSetPinValue(CTRL_PORT,RS,PIN_HIGH);
	DIO_voidSetPinValue(CTRL_PORT,RW,PIN_LOW);
	DIO_voidSetPortValue(DATA_PORT,Copy_u8Data);
	DIO_voidSetPinValue(CTRL_PORT,EN, PIN_HIGH);
	_delay_ms(1);
	DIO_voidSetPinValue(CTRL_PORT,EN,PIN_LOW);
}
static void CLCD_voidSendCmd(u8 Copy_u8Cmd){
	DIO_voidSetPinValue(CTRL_PORT,RS,PIN_LOW);
	DIO_voidSetPinValue(CTRL_PORT,RW,PIN_LOW);
	DIO_voidSetPortValue(DATA_PORT,Copy_u8Cmd);
	DIO_voidSetPinValue(CTRL_PORT,EN, PIN_HIGH);
	_delay_ms(1);
	DIO_voidSetPinValue(CTRL_PORT,EN,PIN_LOW);
}
void CLCD_voidInit(void){
	_delay_ms(40);
	CLCD_voidSendCmd(0b00111000);
	_delay_ms(1);
		CLCD_voidSendCmd(0b00001100);
		_delay_ms(1);
			CLCD_voidSendCmd(1);
			_delay_ms(2);



}
void CLCD_voidSendString(const char *PcString ){
	u8 Local_iterator=0;
	while(PcString[Local_iterator]!='\0'){
		CLCD_voidSendData(PcString[Local_iterator]);
		Local_iterator++;


	}


}
void CLCD_voidSitPosition(u8 Copy_u8x,u8 Copy_u8y ){
	u8 Local_u8Position;
	switch(Copy_u8x){
	case 0:Local_u8Position=Copy_u8y;break;
	case 1:Local_u8Position=Copy_u8y+LINE1_BASE;break;


	}
	CLCD_voidSendCmd(Local_u8Position+MSB);
}

void CLCD_voidSendSpecialChar(u8 *Pu8_arr,u8 Copy_u8BlockNum, u8 Copy_u8x,u8 Copy_u8y){

	u8 Local_u8StartingAddress =Copy_u8BlockNum*8;
	CLCD_voidSendCmd(Local_u8StartingAddress+CGRAM_CONS);

	for(u8 Localu8Iterator=0;Localu8Iterator<8;Localu8Iterator++){
		CLCD_voidSendData(Pu8_arr[Localu8Iterator]);
	}

	CLCD_voidSitPosition(Copy_u8x,Copy_u8y);

	CLCD_voidSendData(Copy_u8BlockNum);


}

void CLCD_voidClearDisplay(void){

	CLCD_voidSendCmd(1);
}
