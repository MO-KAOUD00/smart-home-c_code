
#ifndef EEPROM_INTERFACE_H_
#define EEPROM_INTERFACE_H_

#define EEPROM_DEVICE1      0
#define EEPROM_DEVICE2      1

void EEPROM_voidWriteByte(u16 Copy_u16LocationAddress, u8 Copy_u8Byte, u8 Copy_u8DeviceNumber);

u8 EEPROM_u8ReadByte(u16 Copy_u16LocationAddress, u8 Copy_u8DeviceNumber);

#endif /* EEPROM_INTERFACE_H_ */
