/******************************************************************************
 * Debugging log via MSP432 UART
 *
 * Description: To initialize UART for serial connection between MPS
 *              and Raspberry pi in order to communicate between each other.
 *
 *               MSP432P401
 *             -----------------
 *            |                 |
 *            |                 |
 *       RST -|     P1.3/UCA0TXD|----> Ultrasonic
 *            |                 |
 *            |     P1.2/UCA0RXD|<---- Ultrasonic
 *            |                 |
 *
 *******************************************************************************/
#pragma once
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* UART Configuration - asynchronous serial communication in which
 *                      data format and transmission speeds are configurable*/
const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 // SMCLK Clock Source
        78,                                             // BRDIV = 78
        2,                                              // UCxBRF = 2
        0,                                              // UCxBRS = 0
        EUSCI_A_UART_ODD_PARITY,                        // ODD Parity
        EUSCI_A_UART_LSB_FIRST,                         // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,                      // One stop bit
        EUSCI_A_UART_MODE,                              // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                          // 8 bit data length
};

//own printf for embedded system will not print on debug console
void uPrintf(unsigned char * TxArray)
{
    unsigned short i = 0;
    while(*(TxArray+i))
    {
        UART_transmitData(EUSCI_A0_BASE, *(TxArray+i));  // Write the character at the location specified by pointer
        i++;                                             // Increment pointer to point to the next character
    }
}

//initialize UART configuration that has been set
void initUART()
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_enableModule(EUSCI_A0_BASE);
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
    Interrupt_enableMaster();
}
