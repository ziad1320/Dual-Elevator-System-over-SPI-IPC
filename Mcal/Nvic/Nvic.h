/**
 * Nvic.h
 *
 * Description: Public API for the Nested Vectored Interrupt Controller
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef NVIC_H
#define NVIC_H

#include "Std_Types.h"

/* * Priority Levels (0 to 15)
 * Lower number = Higher Urgency
 */
#define NVIC_PRIORITY_HIGHEST   0U
#define NVIC_PRIORITY_HIGH      4U
#define NVIC_PRIORITY_MEDIUM    8U
#define NVIC_PRIORITY_LOW       12U

/* ========================================================================= */
/* FUNCTION PROTOTYPES                                                       */
/* ========================================================================= */

/**
 * @brief Enables a specific Interrupt Request (IRQ) in the NVIC.
 * @param IrqNumber The IRQ number (0 to 81 for STM32F401).
 */
void Nvic_EnableIrq(uint8 IrqNumber);

/**
 * @brief Disables a specific Interrupt Request (IRQ) in the NVIC.
 * @param IrqNumber The IRQ number.
 */
void Nvic_DisableIrq(uint8 IrqNumber);

/**
 * @brief Sets the priority of an IRQ to satisfy the Emergency Stop requirement.
 * @param IrqNumber The IRQ number.
 * @param Priority Priority level (0-15). 0 is the highest priority.
 */
void Nvic_SetPriority(uint8 IrqNumber, uint8 Priority);

#endif /* NVIC_H */