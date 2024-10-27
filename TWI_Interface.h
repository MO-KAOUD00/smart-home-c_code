
#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_

void TWI_voidMasterInit(u8 Copy_MasterAddress);

void TWI_voidSlaveInit(u8 Copy_SlaveAddress);

void TWI_voidStartCondition();

void TWI_voidSendSlaveAddressWithWrite(u8 Copy_u8SlaveAddress);

void TWI_voidSendSLaveAddressWithRead(u8 Copy_u8SlaveAddress);

void TWI_voidSendData(u8 Copy_u8Data);

u8 TWI_u8RecieveData();

void TWI_voidStopCondition();

#endif /* TWI_INTERFACE_H_ */
