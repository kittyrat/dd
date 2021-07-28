/**
 * Lecture13
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>

#define FOS  2e6
#define FCY  FOS/2
#include <libpic30.h>

/*
 * Display code (HEX)
 */

const uint8_t disp_hex[] = {
    0x3F,   // 0
    0x06,   // 1
    0x5B,   // 2
    0x4F,   // 3
    0x66,   // 4
    0x6D,   // 5
    0x7D,   // 6
    0x07,   // 7
    0x7F,   // 8
    0x6F,   // 9
    0x77,   // A (10)
    0x7C,   // b (11)
    0x39,   // C (12)
    0x5E,   // d (13)
    0x79,   // E (14)
    0x71    // F (15)
};



/*
 * Set MCU Clock Frequency (FOS) to 2MHz
 * Check the #define FOS
 */
void System_Clock_Init(void) {
    CLKDIVbits.RCDIV = 0x2;
}

/*
 * Set IO Ports
 *  - PORTB<11:0>  as digital output ports
 *  - PORTB<15:12> as digital input ports
 */
void System_IO_Init(void) {
    AD1PCFG |= 0x1FFF;
    TRISB = 0xF000;
    LATB &= 0xF000;
}


uint8_t disp_buf[4] = {0x00, 0x00, 0x00, 0x00};

void Display_Multiplex(void) {
    static uint8_t idx = 0;
    LATB &= 0xF000;             // Clear data and digits
    LATB |= 1<<(8+idx);         // Turn on Digit idx
    //LATB |= disp_hex[idx];    // Send display code to 7-segment
    LATB |= disp_buf[idx];      // Send display code to 7-segment
    idx = (idx+1)%4;            // 0, 1, 2, 3
}



/**
 * Main function
 */
int main(void) {

    System_Clock_Init();
    System_IO_Init();


    disp_buf[0] = disp_hex[3];
    disp_buf[1] = disp_hex[6];
    disp_buf[2] = disp_hex[1];
    disp_buf[3] = disp_hex[0];

     while(1) {

        Display_Multiplex();
        __delay_ms(10);              // Delay

    }
}

