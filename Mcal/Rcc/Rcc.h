/**
 * Rcc.h
 *
 * Description: Public API and Peripheral IDs for STM32F401xE
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef RCC_H
#define RCC_H

#include "Std_Types.h"

// Bus Identifiers
#define RCC_AHB1 0UL
#define RCC_AHB2 1UL
#define RCC_APB1 2UL
#define RCC_APB2 3UL

/* ======================================================= */
/* ELEVATOR PROJECT PERIPHERAL IDs                         */
/* ======================================================= */

// --- GPIO Ports ---
#define RCC_GPIOA (RCC_AHB1 * 32 + 0UL) // PWM, SPI, UART
#define RCC_GPIOB (RCC_AHB1 * 32 + 1UL) // Cabin & Sensor EXTI
#define RCC_GPIOC (RCC_AHB1 * 32 + 2UL) // Hallway EXTI (Master only)

// --- DMA (For Telemetry Bonus) ---
#define RCC_DMA1 (RCC_AHB1 * 32 + 21UL) // USART2 TX DMA

// --- Timers ---
#define RCC_TIM2 (RCC_APB1 * 32 + 0UL) // Base Timer (Non-blocking FSM delays)
#define RCC_TIM1 (RCC_APB2 * 32 + 0UL) // Advanced Timer (10kHz PWM Motor)

// --- Communications ---
#define RCC_USART2 (RCC_APB1 * 32 + 17UL) // Telemetry to PC
#define RCC_SPI1 (RCC_APB2 * 32 + 12UL)   // IPC Master/Slave Link

// --- System Configuration ---
#define RCC_SYSCFG (RCC_APB2 * 32 + 14UL) // Required to route EXTI lines

/* ======================================================= */
/* FUNCTION PROTOTYPES                                     */
/* ======================================================= */

void Rcc_Init(void);
void Rcc_Enable(uint8 PeripheralId);
void Rcc_Disable(uint8 PeripheralId);

#endif /* RCC_H */