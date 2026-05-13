/**
* Dma.c
 *
 * Description: Implementation of DMA1 Driver
 * Project    : Collaborative Dual-Elevator System
 * Author     : ziad
 */

#include "Dma.h"
#include "Dma_Private.h"

#include "../RCC/Rcc.h"

void Dma2_Stream7_Init(uint32 PeripheralAddress) {
    // Enable DMA2 Clock (AHB1 bit 22)
    Rcc_Enable(22);

    // 1. Ensure the stream is disabled before configuring
    DMA2_REG->STREAM[7].CR &= ~(1UL << DMA_CR_EN);

    // Wait until the EN bit is physically cleared by hardware
    while (DMA2_REG->STREAM[7].CR & (1UL << DMA_CR_EN));

    // 2. Set the Peripheral Address dynamically (e.g., USART1 DR)
    DMA2_REG->STREAM[7].PAR = PeripheralAddress;

    // 3. Configure the Control Register (CR)
    // - Select Channel 4 (Bits 25:27 = 100) -> Mapped to USART1 TX
    // - Memory to Peripheral direction (Bits 6:7 = 01)
    // - Enable Memory Increment mode (Bit 10 = 1) -> Advances through your string
    uint32 cr_config = 0;
    cr_config |= (4UL << DMA_CR_CHSEL);
    cr_config |= (1UL << DMA_CR_DIR);
    cr_config |= (1UL << DMA_CR_MINC);

    DMA2_REG->STREAM[7].CR = cr_config;
}

void Dma2_Stream7_Send(uint32 SourceAddress, uint16 Length) {
    // 1. Disable the stream before loading new data
    DMA2_REG->STREAM[7].CR &= ~(1UL << DMA_CR_EN);

    // Wait for the hardware to confirm it has stopped
    while (DMA2_REG->STREAM[7].CR & (1UL << DMA_CR_EN));

    // 2. Clear any lingering Transfer Complete flags for Stream 7 in the HIFCR register
    DMA2_REG->HIFCR |= (1UL << DMA_HIFCR_CTCIF7);

    // 3. Load the Memory Address (Where the string lives in RAM)
    DMA2_REG->STREAM[7].M0AR = SourceAddress;

    // 4. Load the number of bytes to transfer
    DMA2_REG->STREAM[7].NDTR = Length;

    // 5. Fire! Enable the stream.
    DMA2_REG->STREAM[7].CR |= (1UL << DMA_CR_EN);
}
