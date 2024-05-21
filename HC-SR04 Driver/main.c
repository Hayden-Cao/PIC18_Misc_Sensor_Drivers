#include <xc.h>
#include <pic18f4321.h>
#include "pic18f4321-Config.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000

#define trigger_bit PORTBbits.RB0
#define echo_bit PORTBbits.RB1
#define LED_bit PORTCbits.RC0

void __interrupt() echo_ISR(void);
unsigned int distance = 0;

void main (void)
{
    ADCON1 = 0x0F;
    OSCCON = 0x60;
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 1;
    TRISC = 0;
    TRISD = 0;
    PORTD = 0;
    PORTC = 0;
    trigger_bit = 0;
    RCONbits.IPEN = 0;
    INTCON3bits.INT1E = 1;
    INTCON3bits.INT1F = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    LCD_init();
    
    T1CON = 0xF4;
    TMR1H = 0;
    TMR1L = 0;
    
    
    for(;;)
    {
        LCD_cursor_set(1,1);
        LCD_write_string("Distance: ");
        LCD_cursor_set(2,1);
        LCD_write_string("No Target Found "); //default case to eliminate flickering
        TMR1H = 0;
        TMR1L = 0;
        LED_bit = 0;
        distance = 0;
        
        trigger_bit = 0;
        __delay_ms(10);
        trigger_bit = 1;
        __delay_us(10);
        trigger_bit = 0;   
        
    }      
    
}

void __interrupt() echo_ISR(void)
{
    
        T1CONbits.TMR1ON = 1;
        while(echo_bit);
        T1CONbits.TMR1ON = 0;
        distance = (unsigned int)((TMR1H << 8) | TMR1L);
        LCD_cursor_set(1,1);
        distance = 8 * distance / 58; //we used a prescaler of 1:8 so the distance number is 8 times smaller than what it is supposed to be for conversion
        LCD_write_string("Distance: ");
        LCD_write_variable(distance, 3);
        LCD_write_string(" cm");
        
        if(distance < 50)
        {
            LCD_cursor_set(2,1);
            LCD_write_string("Target Detected ");
            LED_bit = 1;
            __delay_ms(1000);
        } else
        {
            LCD_cursor_set(2,1);
            LCD_write_string("No Target Found ");
            __delay_ms(60);
        }
        INTCON3bits.INT1F = 0;
        
}


