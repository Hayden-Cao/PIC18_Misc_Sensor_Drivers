#include <xc.h>
#include <math.h>
#include "LCD.h"
#include "pic18f4321-Config.h"

#define _XTAL_FREQ 4000000
#define DHT11_pin_out PORTAbits.RA0
#define DHT11_pin_mode TRISAbits.TRISA0
#define output 0
#define input 1

typedef unsigned char u_char;

void main (void)
{
    u_char data[40];
    u_char humidity_int, humidity_deci, temp_int, temp_deci, check_sum, counter, error_hold = 0;

    enum States { MCU_Start_Signal = 1, DHT11_Response, Read_Data, Process_Data, Error_Check, Display_Data};
    OSCCON = 0x60; // 4 MHz Clk
    ADCON1 = 0x0F; // digital IO
    
    // Port C to output the value of state for debugging purposes
    TRISC = 0;
    PORTC = 0;
    
    DHT11_pin_mode = input;
    T0CON = 0x48; // 8 bit counter, 1:1 prescaler, rising edge
    TMR0L = 0;
    LCD_init(); 
    
    
    enum States myState = MCU_Start_Signal;
    
    for(;;)
    {
        switch(myState)
        {
            
            case MCU_Start_Signal:
                
                
                PORTC = myState;
                
                DHT11_pin_mode = output;
                DHT11_pin_out = 0;
                __delay_ms(20); // datasheet says at least 18 ms
                DHT11_pin_out = 1;
                DHT11_pin_mode = input;
                while(DHT11_pin_out);   
                myState = DHT11_Response;
                
                break;
            
            
            case DHT11_Response:
                PORTC = myState;
                while(!DHT11_pin_out);
                while(DHT11_pin_out);
                
                myState = Read_Data;
                
                break;
            
            case Read_Data:
                
                PORTC = myState;
                
                for (u_char i = 0; i < 40; i++)
                {
                    while(!DHT11_pin_out);
                    T0CONbits.TMR0ON = 1;
                    while(DHT11_pin_out);
                    T0CONbits.TMR0ON = 0;
                    
                    if (TMR0L > 20 && TMR0L < 30)
                        data[i] = 0;
                    else if (TMR0L > 65 && TMR0L < 75)
                        data[i] = 1;
                    else
                        data[i] = 0;
                                
                    TMR0L = 0;
                    
                }
                
                myState = Process_Data;
                
                break;
            
            case Process_Data:
                
                PORTC = myState;
                humidity_int = 0;
                humidity_deci = 0;
                temp_int = 0;
                temp_deci = 0;
                check_sum = 0;
                u_char i = 0;
  
                
                for(i = 0; i < 8; i++)
                {
                    humidity_int |= data[i];

                    if (i < 7)
                        humidity_int = humidity_int << 1;

                }

                for (i = 8; i < 16; i++)
                {
                    humidity_deci |= data[i];

                    if (i < 15)
                        humidity_deci = humidity_deci << 1;

                }

                for (i = 16; i < 24; i++)
                {
                    temp_int |= data[i];

                    if(i < 23)
                        temp_int = temp_int << 1;

                }

                for (i = 24; i < 32; i++)
                {
                    temp_deci |= data[i];

                    if(i < 31)
                        temp_deci = temp_deci << 1;

                }

                for (i = 32; i < 40; i++)
                {
                    check_sum |= data[i];

                    if(i < 39)
                        check_sum = check_sum << 1;

                }
                
                myState = Error_Check;
                
            break;
            
            case Error_Check:
                
                error_hold = temp_deci + temp_int + humidity_int + humidity_deci;
                
                if (error_hold == check_sum)
                {
                    myState = Display_Data;
                } 
                else
                {
                    __delay_ms(900);
                    myState = MCU_Start_Signal;
                }
                
                break;
            
            case Display_Data:
                
                    PORTC = myState;
                    temp_int = round(((temp_int * 9) / 5) + 32);
                    temp_deci = temp_deci * 9 / 5;
                    
                    if (temp_deci >= 10)
                    {
                        temp_int++;
                        temp_deci -= 10;
                    }
                    
                    LCD_clear();
                    LCD_cursor_set(1,1);
                    LCD_write_string("Temp: ");
                    LCD_write_variable(temp_int, 3);
                    LCD_write_string(".");

                    if (temp_deci > 10)
                        LCD_write_variable(temp_deci, 2);
                    else 
                        LCD_write_variable(temp_deci, 1);
                    
                    LCD_write_string(" F");

                    LCD_cursor_set(2,1);
                    LCD_write_string("Humidity: ");
                    LCD_write_variable(humidity_int, 2);
                    LCD_write_string(".");

                    if (humidity_deci > 10)
                        LCD_write_variable(humidity_deci, 2);
                    else
                        LCD_write_variable(humidity_deci, 1);

                    LCD_write_string("%");
                       
                    __delay_ms(900);
                    myState = MCU_Start_Signal;
            break;
            
            
        }
        
    }
    
    
}