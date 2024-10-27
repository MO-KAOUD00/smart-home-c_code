
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TWI_Interface.h"
#include "EEPROM_Interface.h"
#include <util/delay.h>

void EEPROM_voidWriteByte(u16 Copy_u16LocationAddress, u8 Copy_u8Byte, u8 Copy_u8DeviceNumber)
{
	u8 Local_u8SlaveAddress;
	Local_u8SlaveAddress = (0b01010000) | (Copy_u8DeviceNumber<<2) | (u8)(Copy_u16LocationAddress>>8);

	TWI_voidStartCondition();

	TWI_voidSendSlaveAddressWithWrite(Local_u8SlaveAddress);

	TWI_voidSendData((u8)Copy_u16LocationAddress);

	TWI_voidSendData(Copy_u8Byte);

	TWI_voidStopCondition();

	_delay_ms(10);
}

u8 EEPROM_u8ReadByte(u16 Copy_u16LocationAddress, u8 Copy_u8DeviceNumber)
{

	u8 Local_u8ReturnData;
	u8 Local_u8SlaveAddress;
	Local_u8SlaveAddress = (0b01010000) | (Copy_u8DeviceNumber<<2) | (u8)(Copy_u16LocationAddress>>8);

	TWI_voidStartCondition();

	TWI_voidSendSlaveAddressWithWrite(Local_u8SlaveAddress);

	TWI_voidSendData((u8)Copy_u16LocationAddress);

	TWI_voidStartCondition();

	TWI_voidSendSLaveAddressWithRead(Local_u8SlaveAddress);

	Local_u8ReturnData = TWI_u8RecieveData();

	TWI_voidStopCondition();

	return Local_u8ReturnData;
}
