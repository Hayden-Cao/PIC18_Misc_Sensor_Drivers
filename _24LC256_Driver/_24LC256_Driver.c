#include <xc.h>
#include <pic18f4321.h>
#include "pic18f4321-Config.h"
#include "24LC256_Driver.h"


u_char read_eeprom(u_char addr_high, u_char addr_low)
{
    u_char read_data = 0;
    
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
    SSPBUF = (EEPROM_ADDR << 1);
    while(SSPSTATbits.BF);
    
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;
    }
    
    SSPBUF = addr_high;
    while(SSPSTATbits.BF);
    
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;
    }
    
    SSPBUF = addr_low;
    while(SSPSTATbits.BF);
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;
    }
    
    SSPCON2bits.RSEN = 1;
    while(SSPCON2bits.RSEN);
    SSPBUF = (EEPROM_ADDR << 1) | 1;
    while(SSPSTATbits.BF);
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return 0;
    }
    
    SSPCON2bits.RCEN = 1;
    while(SSPSTATbits.BF == 0);
    read_data = SSPBUF;
    SSPCON2bits.RCEN = 0;
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    while(SSPCON2bits.ACKEN);
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
    
    return read_data;
}


void write_eeprom(u_char data, u_char addr_high, u_char addr_low)
{
    write_protect = 0;
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
    SSPBUF = (EEPROM_ADDR << 1);
    while(SSPSTATbits.BF);
    
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return;
    }
    
    SSPBUF = addr_high;
    while(SSPSTATbits.BF);
    
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return;
    }
    
    SSPBUF = addr_low;
    while(SSPSTATbits.BF);
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return;
    }
    
    SSPBUF = data;
    while(SSPSTATbits.BF);
    // if not acknowledged
    if (SSPCON2bits.ACKSTAT)
    {
        SSPCON2bits.PEN = 1;
        while(SSPCON2bits.PEN);
        return;
    }
    
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
    write_protect = 1;
    
}

void i2c_init()
{
    ADCON1 = 0x0F;
    scl_io = input;
    sda_io = input;
    SSPCON1bits.SSPM3 = 1;
    SSPCON1bits.SSPEN = 1;
    SSPADD = 9;
    wp_io = output;
}
