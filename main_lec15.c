/**
 * Lecture 15
 */


/**
 * Include Files
 */
#include <xc.h>
#include <stdio.h>
#include <stdint.h>


/**
 * CPU Clock Frequency
 */
#define FOS  2e6


/**
 * Instruction Clock Frequency
 */
#define FCY  FOS/2
#include <libpic30.h>


/**
 * Display Hex Codes of 0-F
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
 * Update Display on 7-Segment
 */
void Display_Multiplex(void) {
    static uint8_t idx = 0;
    LATB &= 0xF000;             // Clear DATA<7:0> and DIGIT<3:0>
    LATB |= 1<<(8+idx);         // Turn on DIGIT<idx>
    LATB |= disp_buf[idx];      // Write display code to DATA<7:0>
    idx = (idx+1)%4;            // Next Digit (0, 1, 2, 3, 0, ...)
}

/**
 * Update Display Buffer (decimal format)
 */
void Display_Uint(uint16_t value) {
    int8_t i;
    for(i=3; i>=0; i--) {
        disp_buf[i] = disp_hex[value%10];
        value /= 10;
    }
}

/**
 * Update Display Buffer (hexadecimal format)
 */
void Display_Uhex(uint16_t value) {
    int8_t i;
    for(i=3; i>=0; i--) {
        disp_buf[i] = disp_hex[value%16];
        value /= 16;
    }
}
int main(void) {

    System_Clock_Init();
    System_IO_Init();


    disp_buf[0] = disp_hex[12];
    disp_buf[1] = 0x78;
    disp_buf[2] = 0x50;
    disp_buf[3] = 0x38;

    while(1) {

        Display_Multiplex();
        __delay_ms(10);              // Delay

    }
}

void System_Timer1_Init(void) {

    // // 1) Clock source
    // T1CONbits.TCS = 0;      // 0 = Internal clock (FOSC/2)

    // // 2) Prescaler
    // T1CONbits.TCKPS = 0b00; // 1:1 Input Clock Prescaler

    // // 3) Start T1
    // T1CONbits.TON = 1;      // 1 = Starts 16-bit Timer1

    // // 4) Clear Timer1 (Counter)
    // TMR1 = 0;


    //-----------
    IEC0bits.T1IE   = 0;    // Disable Timer Interrupt
    IFS0bits.T1IF   = 0;    // Clear T1IF
    T1CONbits.TCS   = 0;    // 0 = Internal clock (FOSC/2)
    T1CONbits.TCKPS = 0b00; // 1:1 Input Clock Prescaler
    T1CONbits.TGATE = 0;    // Disable Gate
    PR1 = 5000;
    TMR1 = 0;               // Clear Timer1 (Counter)
    T1CONbits.TON   = 1;    // 1 = Starts 16-bit Timer1
    IEC0bits.T1IE   = 1;    // Enable Timer Interrupt
}

void System_CN_Init(void) {
    CNEN1 |= 0xF<<11;
    IEC1  |= 1<<3;
}


uint8_t tmr1_flag = 0;
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF   = 0;    // Clear T1IF
    tmr1_flag = 1;
}

uint16_t psw_data = 0x0000;
uint8_t  psw_flag = 0;
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    IFS1 &= ~(1<<3);
    psw_flag = 1;
    psw_data = PORTB & 0xF000;
}

/**
 * Main Function
 */
int main(void) {

    System_Clock_Init();        // Initialize System Clock
    System_IO_Init();           // Initialize I/O Ports
    System_Timer1_Init();       // Initialize Timer1
    System_CN_Init();           // Initialize Change Notification (PSW<3:0>)
    Display_Uhex(0x0000);

    uint16_t counter = 0;
    uint16_t sw = 0;
    uint16_t cnt_5ms = 0;
    while(1) {

        if(tmr1_flag != 0) {
            tmr1_flag = 0;
            Display_Multiplex();    // Update display 7-Segment display (every 5mS)
            cnt_5ms++;
        }

        if(cnt_5ms >= 200) {
            cnt_5ms = 0;
            counter++;
            Display_Uint(counter);
        }

        if(psw_flag != 0) {
            psw_flag = 0;
            sw = psw_data;
            if( ( sw & 0x8000 ) == 0) {  // PSW3
                counter += 20;
            }
           
            if( ( sw & 0x2000 ) == 0) {  // PSW1
                counter -= 1;
            }
           
            Display_Uint(counter);
        }
    }
}


/*
Display_Uhex(0xFEDC);

uint16_t cnt = 0;

while(1) {

    cnt++;

    if(cnt >= 350) {
        cnt = 0;
        Display_Multiplex();    // Update display 7-Segment display
    }

    if( (PORTB & 0xF000) != 0xF000) {
        Display_Uhex(PORTB & 0xF000);
    }
}
*/


/*
uint16_t sw = 0xF000;

while(1) {

    // if(TMR1 >= 5000) {
    //     TMR1 = 0;
    //     Display_Multiplex();    // Update display 7-Segment display (every 5mS)
    // }

    if(tmr1_flag != 0) {
        tmr1_flag = 0;
        Display_Multiplex();    // Update display 7-Segment display (every 5mS)
    }

    sw = PORTB & 0xF000;

    switch(sw) {

        case 0xF000:    // 1111 xxxx xxxx xxxx (ALL OFF)
            //
        break;

        case 0xE000:    // 1110 xxxx xxxx xxxx (PSW0)
            Display_Uint(0);
        break;

        case 0xD000:    // 1101 xxxx xxxx xxxx (PSW1)
            Display_Uint(1);
        break;

        case 0xB000:    // 1011 xxxx xxxx xxxx (PSW2)
            Display_Uint(2);
        break;

        case 0x7000:    // 0111 xxxx xxxx xxxx (PSW3)
            Display_Uint(3);
        break;
    }
}
*/

 /*
if( (psw_data & 0x8000) == 0) { // PSW3
    counter += 1000;
    Display_Uint(counter);
    psw_data = 0xF000;
}
if( (psw_data & 0x4000) == 0) { // PSW2
    counter += 100;
    Display_Uint(counter);
    psw_data = 0xF000;
}
if( (psw_data & 0x2000) == 0) { // PSW1
    counter += 10;
    Display_Uint(counter);
    psw_data = 0xF000;
}
if( (psw_data & 0x1000) == 0) { // PSW0
    counter += 1;
    Display_Uint(counter);
    psw_data = 0xF000;
}
*/