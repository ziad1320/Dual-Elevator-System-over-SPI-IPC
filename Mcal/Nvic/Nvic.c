/**
 * Nvic.c
 *
 * Description: Implementation of the NVIC Driver
 * Project    : Collaborative Dual-Elevator System
 */

#include "Nvic.h"
#include "Nvic_Private.h"

void Nvic_EnableIrq(uint8 IrqNumber) {
    if (IrqNumber < 240) { // Safety check against maximum Cortex-M4 interrupts
        // Divide by 32 to find the right ISER array index
        // Modulo 32 to find the specific bit in that register
        NVIC->ISER[IrqNumber / 32] = (1UL << (IrqNumber % 32));
    }
}

void Nvic_DisableIrq(uint8 IrqNumber) {
    if (IrqNumber < 240) {
        NVIC->ICER[IrqNumber / 32] = (1UL << (IrqNumber % 32));
    }
}

void Nvic_SetPriority(uint8 IrqNumber, uint8 Priority) {
    if (IrqNumber < 240) {
        // STM32F401 implements only the upper 4 bits of the priority byte.
        // Therefore, we must shift the 0-15 value left by 4.
        NVIC->IPR[IrqNumber] = (uint8)(Priority << 4);
    }
}