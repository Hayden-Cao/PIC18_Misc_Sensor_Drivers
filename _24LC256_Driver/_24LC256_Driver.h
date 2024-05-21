/* 
 * File:   TC74_Driver.h
 * Author: caoha
 *
 * Created on May 19, 2024, 9:12 AM
 */

#ifndef _24LC256_DRIVER_H
#define	_24LC256_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif


// uint8_t is equivalent to unsigned char
typedef unsigned char u_char;

#define scl_io TRISCbits.TRISC3
#define sda_io TRISCbits.TRISC4
#define write_protect PORTAbits.RA1
#define wp_io TRISAbits.TRISA1

#define output 0
#define input 1

#define EEPROM_ADDR 0b1010111 // 24LC256

void i2c_init();
void write_eeprom(u_char data, u_char addr_high, u_char addr_low);
u_char read_eeprom(u_char addr_high, u_char addr_low);


#ifdef	__cplusplus
}
#endif

#endif	/* TC74_DRIVER_H */
