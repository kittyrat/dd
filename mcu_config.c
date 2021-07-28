/************************************************************
 * PIC24FJ48GA002 Configuration for INC361                  *
 ************************************************************
 * File:    mcu_config.c                                    *
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, KMUTT, Thailand                   *
 ************************************************************
 * Update1:  24 October 2020 (Initial version)              *
 * Update2:  15 November 2020                               *
 ************************************************************/

#pragma config IESO     = OFF       // IESO mode (Two-Speed Start-up) disabled
#pragma config FNOSC    = FRCDIV    // Fast RC Oscillator with Postscaler (FRCDIV)
#pragma config FCKSM    = CSECMD    // Clock switching is enabled, Fail-Safe Clock Monitor is disabled
#pragma config IOL1WAY  = OFF       // IOLOCK may be changed via unlocking sequence
#pragma config OSCIOFNC = ON        // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config JTAGEN   = OFF       // JTAG port is disabled
#pragma config GCP      = ON        // Code protection is enabled for the entire program memory space
#pragma config GWRP     = OFF       // Writes to program memory are allowed
#pragma config BKBUG    = OFF       // Device resets into Operational mode
#pragma config ICS      = PGx3      // Emulator EMUC3/EMUD3 pins are shared with PGC3/PGD3
#pragma config FWDTEN   = OFF       // Watchdog Timer is disabled
#pragma config WINDIS   = ON        // Standard Watchdog Timer enabled,(Windowed-mode is disabled)
#pragma config FWPSA    = PR32      // Prescaler ratio of 1 : 32
#pragma config WDTPS    = PS1024    // 1 : 1,024



/*
 * Check details in the '/Microchip/xc16/v1.60/docs/config_docs/24FJ48GA002.html'
 */


/* EOF */
