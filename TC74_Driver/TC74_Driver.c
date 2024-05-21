#include <xc.h>
#include <stdint.h>
#include "pic18f4321-Config.h"
#include "TC74_Driver.h"

void i2c_init()
{
    ADCON1 = 0x0F;
    scl_io = input;
    sda_io = input;
    SSPCON1bits.SSPM3 = 1;
    SSPCON1bits.SSPEN = 1;
    SSPADD = 9;
}

uint8_t read_temp()
{
    uint8_t temp = 0;

    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
    SSPBUF = TC74_ADDR << 1;
    while(SSPSTATbits.BF);

    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;        
    }    

    SSPBUF = 0x00;
    while(SSPSTATbits.BF);

    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;        
    }  

    SSPCON2bits.RSEN = 1;
    while(SSPCON2bits.RSEN);
    SSPBUF = (TC74_ADDR << 1) | 1;
    while(SSPSTATbits.BF);

    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;        
    }  

    SSPCON2bits.RCEN = 1;
    while(SSPSTATbits.BF == 0);
    temp = SSPBUF;
    SSPCON2bits.RCEN = 0;
    
    
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    while(SSPCON2bits.ACKEN);
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);

    return temp;
}
