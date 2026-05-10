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

    // CRITICAL FOR YOUR TASK: Enable DMA Mode for Transmitter
    // This tells the USART hardware to automatically request the next byte
    // from the DMA controller whenever the Transmit Data Register is empty.
    USART2_REG->CR3 |= (1 << CR3_DMAT);

    // Enable USART2 Peripheral
    USART2_REG->CR1 |= (1 << CR1_UE);
}

void Usart2_TransmitStringTest(const char* Str) {
    uint32 i = 0;

    // Loop until we hit the null-terminator of the string
    while (Str[i] != '\0') {
        // Poll TXE (Transmit Data Register Empty) flag
        // Wait here until the hardware is ready to accept the next byte
        while (!(USART2_REG->SR & (1 << SR_TXE)));

        // Write the byte into the Data Register
        USART2_REG->DR = Str[i];
        i++;
    }

    // Wait for TC (Transmission Complete) to ensure the final byte
    // has physically left the shift register before exiting.
    while (!(USART2_REG->SR & (1 << SR_TC)));

    // Clear TC flag (Done by writing 0 to it in STM32F4)
    USART2_REG->SR &= ~(1 << SR_TC);
}