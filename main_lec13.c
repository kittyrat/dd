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


    disp_buf[0] = disp_hex[1];
    disp_buf[1] = disp_hex[5];
    disp_buf[2] = 0x3e;
    disp_buf[3] = disp_hex[5] | 0x80;

    while(1) {

        Display_Multiplex();
        __delay_ms(100);              // Delay

    }
}


// /**
//  * Main function
//  */
// int main(void) {

//     System_Clock_Init();
//     System_IO_Init();

//     LATB &= 0xF000; // Turn off all display elements

//     uint8_t idx = 0;

//     while(1) {

//         /*
//         for(idx=0; idx<=3; idx++) {
//             LATB |= 1<<(8+idx);         // Turn on Digit idx
//             LATB |= disp_hex[9-idx];    // Send display code to 7-segment
//             __delay_ms(5);              // Delay
//             LATB &= ~(1<<(8+idx));      // Turn off Digit idx
//             LATB &= 0xFF00;             // Clear data
//         }
//         */

//         /*
//         LATB &= 0xF000;             // Clear data and digits
//         LATB |= 1<<(8+idx);         // Turn on Digit idx
//         LATB |= disp_hex[9-idx];    // Send display code to 7-segment
//         idx = (idx+1)%4;            // 0, 1, 2, 3
//         __delay_ms(5);              // Delay
//         //LATB &= 0xF0FF;           // Turn off all Digits
//         //LATB &= 0xFF00;           // Clear data
//         */


//         /*
//         idx++;
//         idx %= 4;                   // 0, 1, 2, 3
//         */

//         /*
//         idx++;                      // 0, 1, 2, 3
//         if(idx >=4) {
//             idx = 0;
//         }
//         */

//         //
//     }
// }


// /**
//  * Main function
//  */
// int main(void) {

//     System_Clock_Init();
//     System_IO_Init();

//     LATB &= 0xF000; // Turn off all display elements

//     uint8_t idx = 0;

//     while(1) {
//         /*
//         LATB |= 1<<8;       // Turn on Digit 0
//         LATB |= 0x06;       // Send display code to 7-segment
//         __delay_ms(5);      // Delay
//         LATB &= ~(1<<8);    // Turn off Digit 0
//         LATB &= 0xFF00;     // Clear data

//         LATB |= 1<<9;       // Turn on Digit 1
//         LATB |= 0x5B;       // Send display code to 7-segment
//         __delay_ms(5);      // Delay
//         LATB &= ~(1<<9);    // Turn off Digit 1
//         LATB &= 0xFF00;     // Clear data

//         LATB |= 1<<10;      // Turn on Digit 2
//         LATB |= 0x4F;       // Send display code to 7-segment
//         __delay_ms(5);      // Delay
//         LATB &= ~(1<<10);   // Turn off Digit 2
//         LATB &= 0xFF00;     // Clear data

//         LATB |= 1<<11;      // Turn on Digit 3
//         LATB |= 0x66;       // Send display code to 7-segment
//         __delay_ms(5);      // Delay
//         LATB &= ~(1<<11);   // Turn off Digit 3
//         LATB &= 0xFF00;     // Clear data
//         */

//         /*
//         for(idx=0; idx<=3; idx++) {
//             LATB |= 1<<(8+idx);     // Turn on Digit idx
//             LATB |= 0x06;           // Send display code to 7-segment
//             __delay_ms(5);          // Delay
//             LATB &= ~(1<<(8+idx));  // Turn off Digit idx
//             LATB &= 0xFF00;         // Clear data
//         }
//         */

//         /*
//         for(idx=0; idx<=3; idx++) {
//             LATB |= 1<<(8+idx);     // Turn on Digit idx
//             LATB |= disp_hex[9-idx];  // Send display code to 7-segment
//             __delay_ms(5);          // Delay
//             LATB &= ~(1<<(8+idx));  // Turn off Digit idx
//             LATB &= 0xFF00;         // Clear data
//         }
//         */

//         //

//         /*
//         for(idx=11; idx>=8; idx--) {
//             LATB |= 1<<idx;         // DIGIT<idx-8> ON
//             __delay_ms(500);
//             LATB &= ~(1<<idx);      // DIGIT3<idx> OFF
//         }
//         */
//     }
// }