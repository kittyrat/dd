#include <xc.h>
#include <stdio.h>
#include <stdint.h>
//ecc-pic24-cli -build

void System_Clock_Init(void) {
    CLKDIVbits.RCDIV = 0x2;
}

/*
 * Set IO Ports, PORTB<11:0) as digital output ports
 */
void System_IO_Init(void) {
    AD1PCFG |= 0x1FFF;
    TRISB = 0xF000;
    LATB &= 0xF000;
}


int main(void) {
    
    System_Clock_Init();
    System_IO_Init();
    LATB &= 0xF000;
    LATB |= 0x00FF; // 8.
    while(1) {
}
