/**
 * Usart.c
 *
 * Description: Implementation of STM32F401xE USART2 Driver (Telemetry)
 * Project    : Collaborative Dual-Elevator System
 * Author     : ziad
 */

#include "Usart.h"
#include "Usart_Private.h"
#include "Gpio.h"


void Usart2_Init(void) {
    // Configure PA2 for USART2 TX (Alternate Function 7)
    Gpio_Init(GPIO_A, 2, GPIO_AF, GPIO_PUSH_PULL);
    Gpio_SetAF(GPIO_A, 2, GPIO_AF7);

    // Clear Control Registers (Good practice before configuration)
    USART2_REG->CR1 = 0;
    USART2_REG->CR2 = 0;
    USART2_REG->CR3 = 0;

    // Frame Configuration: 8 data bits, 1 stop bit, no parity
    // M bit = 0 (8-bit word), OVER8 = 0 (16x oversampling)
    USART2_REG->CR1 &= ~((1 << CR1_M) | (1 << CR1_OVER8));

    // Baud Rate Configuration (9600 Baud at 16MHz System Clock)
    // Mantissa = 104 (0x68), Fraction = 3 (0x3) -> 0x683
    USART2_REG->BRR = 0x683;

    // Enable the Transmitter
    USART2_REG->CR1 |= (1 << CR1_TE);

    // Enable DMA Mode for Transmitter
    // This tells the USART hardware to automatically request the next byte
    // from the DMA controller whenever the Transmit Data Register is empty.
    USART2_REG->CR3 |= (1 << CR3_DMAT);

    // Enable USART2 Peripheral
    USART2_REG->CR1 |= (1 << CR1_UE);
}