/**
* Usart.h
 *
 * Description: Public API for STM32F401xE USART2 Driver (Telemetry)
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef USART_H
#define USART_H

#include "Std_Types.h"

/* ========================================================================= */
/* FUNCTION PROTOTYPES                                                       */
/* ========================================================================= */

/**
 * @brief Initializes USART2 on PA2 (TX) for 9600 baud, 8-N-1.
 * Configures the USART to trigger DMA requests for transmission.
 */
void Usart2_Init(void);


#endif /* USART_H */