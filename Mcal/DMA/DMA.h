/**
* Dma.h
 *
 * Description: Public API for DMA1 (Specifically configured for USART2 TX)
 * Project    : Collaborative Dual-Elevator System
 * Author     : ziad
 */

#ifndef DMA_H
#define DMA_H

#include "Std_Types.h"

/* ========================================================================= */
/* FUNCTION PROTOTYPES                                                       */
/* ========================================================================= */

/**
 * @brief Initializes DMA2 Stream 7 for USART1 Transmission.
 * Maps Channel 4, sets Memory-to-Peripheral direction, and enables Memory Increment.
 */
void Dma2_Stream7_Init(uint32 PeripheralAddress);

/**
 * @brief Triggers a background DMA transfer.
 * @param SourceAddress The memory address of the string/data array to send.
 * @param Length The number of bytes to send.
 */
void Dma2_Stream7_Send(uint32 SourceAddress, uint16 Length);

#endif /* DMA_H */