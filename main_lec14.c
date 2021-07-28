/**
 * Lecture14
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>

#define FOS  2e6
#define FCY  FOS/2
#include <libpic30.h>

/**
 * Display Code 0-F (HEX)
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

/**
 * Set MCU Clock Frequency (FOS) to 2MHz
 * Check the #define FOS
 */
void System_Clock_Init(void) {
    CLKDIVbits.RCDIV = 0x2;
}

/**
 * Set IO Ports
 *  - PORTB<11:0>  as digital output ports
 *  - PORTB<15:12> as digital input ports
 */
void System_IO_Init(void) {
    AD1PCFG |= 0x1FFF;
    TRISB = 0xF000;
    LATB &= 0xF000;
}


/**
 * Display Buffer
 */
uint8_t disp_buf[4] = {0xFF, 0xFF, 0xFF, 0xFF};


/**
 * Update Display
 */
void Display_Multiplex(void) {
    static uint8_t idx = 0;
    LATB &= 0xF000;             // Clear DATA<7:0> and DIGIT<3:0>
    LATB |= 1<<(8+idx);         // Turn on DIGIT<idx>
    LATB |= disp_buf[idx];      // Write display code to DATA<7:0>
    idx = (idx+1)%4;            // Next Digit (0, 1, 2, 3, 0, ...)
}

void Display_Uint(uint16_t value) {
    int8_t i;
    for(i=3; i>=0; i--) {
        disp_buf[i] = disp_hex[value%10];
        value /= 10;
    }
}

void Display_Uhex(uint16_t value) {
    int8_t i;
    for(i=3; i>=0; i--) {
        disp_buf[i] = disp_hex[value%10];
        value /= 10 ;
    }
}

/**
 * Main Function
 */
int main(void) {

    System_Clock_Init();
    System_IO_Init();

    uint16_t counter = 0;
    uint16_t k = 0;

    Display_Uint(counter);

    while(1) {

        Display_Multiplex();
        __delay_ms(5);          // Delay 5mS

        if(++k>100) {
            counter++;
            k=0;
            Display_Uhex(counter);
        }
    }
}


/*
uint16_t value = 5698;
// 1  2  3  4
uint16_t i0, i1, i2, i3;
i3 = value%10;      // value=1234,  i3=4
value/=10;          // value=123
i2 = value%10;      // value=123,   i2=3
value/=10;          // value=12
i1 = value%10;      // value=12,    i1=2
value/=10;          // value=1
i0 = value%10;      // value=1,     i0=1
value/=10;          // value=0
disp_buf[0] = disp_hex[i0];
disp_buf[1] = disp_hex[i1];
disp_buf[2] = disp_hex[i2];
disp_buf[3] = disp_hex[i3];
*/


/*
// uint16_t value = 9876;
// uint8_t i;
// for(i=0; i<=3; i++) {
//     disp_buf[3-i] = disp_hex[value%10];
//     value /= 10;
// }
uint16_t value = 9876;
//uint8_t i;    // 0-255 ... 0 -> 255
int8_t i;       // -128,+127 ... 0 -> -1
for(i=3; i>=0; i--) {
    disp_buf[i] = disp_hex[value%10];
    value /= 10;
}
*/