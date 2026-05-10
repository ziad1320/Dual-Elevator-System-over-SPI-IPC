/**
* Dma.c
 *
 * Description: Implementation of DMA1 Driver
 * Project    : Collaborative Dual-Elevator System
 * Author     : ziad
 */

#include "Dma.h"
#include "Dma_Private.h"

void Dma1_Stream6_Init(uint32 PeripheralAddress) {
    // 1. Ensure the stream is disabled before configuring
    DMA1_REG->STREAM[6].CR &= ~(1UL << DMA_CR_EN);

    // Wait until the EN bit is physically cleared by hardware
    while (DMA1_REG->STREAM[6].CR & (1UL << DMA_CR_EN));

    // 2. Set the Peripheral Address dynamically (e.g., USART2 DR)
    // The DMA now blindly pushes data to whatever address you pass in here!
    DMA1_REG->STREAM[6].PAR = PeripheralAddress;

    // 3. Configure the Control Register (CR)
    // - Select Channel 4 (Bits 25:27 = 100) -> Mapped to USART2 TX
    // - Memory to Peripheral direction (Bits 6:7 = 01)
    // - Enable Memory Increment mode (Bit 10 = 1) -> Advances through your string
    // Note: Peripheral Increment is OFF, so it always writes to the same DR address.
    uint32 cr_config = 0;
    cr_config |= (4UL << DMA_CR_CHSEL);
    cr_config |= (1UL << DMA_CR_DIR);
    cr_config |= (1UL << DMA_CR_MINC);

    DMA1_REG->STREAM[6].CR = cr_config;
}

void Dma1_Stream6_Send(uint32 SourceAddress, uint16 Length) {
    // 1. Disable the stream before loading new data
    DMA1_REG->STREAM[6].CR &= ~(1UL << DMA_CR_EN);

    // Wait for the hardware to confirm it has stopped
    while (DMA1_REG->STREAM[6].CR & (1UL << DMA_CR_EN));

    // 2. Clear any lingering Transfer Complete flags for Stream 6 in the HIFCR register
    DMA1_REG->HIFCR |= (1UL << DMA_HIFCR_CTCIF6);

    // 3. Load the Memory Address (Where the string lives in RAM)
    DMA1_REG->STREAM[6].M0AR = SourceAddress;

    // 4. Load the number of bytes to transfer
    DMA1_REG->STREAM[6].NDTR = Length;

    // 5. Fire! Enable the stream. The DMA hardware instantly takes over.
    DMA1_REG->STREAM[6].CR |= (1UL << DMA_CR_EN);
}