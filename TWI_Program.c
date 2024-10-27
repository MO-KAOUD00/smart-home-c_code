
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TWI_Register.h"
#include "TWI_Interface.h"

void TWI_voidMasterInit(u8 Copy_MasterAddress)
{
	TWBR = 12;

	CLR_BIT(TWSR,TWSR_TWPS0);
	CLR_BIT(TWSR,TWSR_TWPS1);

	SET_BIT(TWCR,TWCR_TWEA);

	TWAR = Copy_MasterAddress<<1;

	SET_BIT(TWCR,TWCR_TWEN);
}

void TWI_voidSlaveInit(u8 Copy_SlaveAddress)
{
	SET_BIT(TWCR,TWCR_TWEA);

	TWAR = Copy_SlaveAddress<<1;

	SET_BIT(TWCR,TWCR_TWEN);
}

void TWI_voidStartCondition()
{
	SET_BIT(TWCR,TWCR_TWSTA);

	SET_BIT(TWCR,TWCR_TWINT);

	while( GET_BIT(TWCR,TWCR_TWINT) == 0 );
}

void TWI_voidSendSlaveAddressWithWrite(u8 Copy_u8SlaveAddress)
{
	TWDR = Copy_u8SlaveAddress<<1;
	CLR_BIT(TWDR,0);

	CLR_BIT(TWCR,TWCR_TWSTA);

	SET_BIT(TWCR,TWCR_TWINT);

	while( GET_BIT(TWCR,TWCR_TWINT) == 0 );
}

void TWI_voidSendSLaveAddressWithRead(u8 Copy_u8SlaveAddress)
{
	TWDR = Copy_u8SlaveAddress<<1;
	SET_BIT(TWDR,0);

	CLR_BIT(TWCR,TWCR_TWSTA);

	SET_BIT(TWCR,TWCR_TWINT);

	while( GET_BIT(TWCR,TWCR_TWINT) == 0 );
}

void TWI_voidSendData(u8 Copy_u8Data)
{
	TWDR = Copy_u8Data;

	SET_BIT(TWCR,TWCR_TWINT);

	while( GET_BIT(TWCR,TWCR_TWINT) == 0 );
}

u8 TWI_u8RecieveData()
{
	SET_BIT(TWCR,TWCR_TWINT);

	while( GET_BIT(TWCR,TWCR_TWINT) == 0 );

	return TWDR;
}

void TWI_voidStopCondition()
{
	SET_BIT(TWCR,TWCR_TWSTO);
}
