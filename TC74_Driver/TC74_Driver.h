/* 
 * File:   TC74_Driver.h
 * Author: caoha
 *
 * Created on May 19, 2024, 9:12 AM
 */

#ifndef TC74_DRIVER_H
#define	TC74_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define scl_io TRISCbits.TRISC3
#define sda_io TRISCbits.TRISC4

#define output 0
#define input 1

#define TC74_ADDR  0x4C

void i2c_init();
uint8_t read_temp();


#ifdef	__cplusplus
}
#endif

#endif	/* TC74_DRIVER_H */

