/**
 * Exti.c
 *
 * Description: Implementation of External Interrupts
 * Project    : Collaborative Dual-Elevator System
 */

#include "Exti.h"
#include "Exti_Private.h"

// Array of function pointers to hold application callbacks
static ExtiCallback ExtiCallbacks[16] = {0};

// Maps EXTI Line Number (0-15) to STM32F401 NVIC IRQ Position
static const uint8 ExtiLineNumberNvicMap[16] = {
    6, 7, 8, 9, 10,                 // EXTI 0 to 4
    23, 23, 23, 23, 23,             // EXTI 5 to 9 (Shared IRQ23)
    40, 40, 40, 40, 40, 40          // EXTI 10 to 15 (Shared IRQ40)
};

void Exti_Init(uint8 LineNumber, uint8 PortName, uint8 EdgeType, ExtiCallback Callback) {
    // 1. Configure SYSCFG EXTICR multiplexer
    uint8 sysConfigIndex = LineNumber / 4;
    uint8 sysConfigLogicalBitPosition = LineNumber % 4;

    // Clear the 4 bits for this line, then set the new port
    SYSCFG->EXTICR[sysConfigIndex] &= ~(0x0F << (sysConfigLogicalBitPosition * 4));
    SYSCFG->EXTICR[sysConfigIndex] |=  (PortName << (sysConfigLogicalBitPosition * 4));

    // 2. Register the callback
    ExtiCallbacks[LineNumber] = Callback;

    // 3. Clear trigger configurations
    EXTI->RTSR &= ~(0x01 << LineNumber);
    EXTI->FTSR &= ~(0x01 << LineNumber);

    // 4. Set requested trigger Edge
    switch (EdgeType) {
        case EXTI_EDGE_RISING:
            EXTI->RTSR |= (0x01 << LineNumber);
            break;
        case EXTI_EDGE_FALLING:
            EXTI->FTSR |= (0x01 << LineNumber);
            break;
        case EXTI_EDGE_BOTH:
            EXTI->RTSR |= (0x01 << LineNumber);
            EXTI->FTSR |= (0x01 << LineNumber);
            break;
        default:
            break;
    }
}

void Exti_Enable(uint8 LineNumber) {
    // Unmask the EXTI line
    EXTI->IMR |= (0x01 << LineNumber);
    
    // Enable the corresponding IRQ in the NVIC
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_ISER[irqNumber / 32] |= (0x01 << (irqNumber % 32));
}

void Exti_Disable(uint8 LineNumber) {
    // Mask the EXTI line
    EXTI->IMR &= ~(0x01 << LineNumber);
    
    // Disable the corresponding IRQ in the NVIC
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_ICER[irqNumber / 32] |= (0x01 << (irqNumber % 32));
}

/* ========================================================================= */
/* INTERRUPT SERVICE ROUTINES (ISRs)                                         */
/* ========================================================================= */

void EXTI0_IRQHandler(void) {
    if (ExtiCallbacks[0] != 0) { ExtiCallbacks[0](); }
    EXTI->PR |= (0x01 << 0); // Clear pending bit
}

void EXTI1_IRQHandler(void) {
    if (ExtiCallbacks[1] != 0) { ExtiCallbacks[1](); }
    EXTI->PR |= (0x01 << 1); 
}

void EXTI2_IRQHandler(void) {
    if (ExtiCallbacks[2] != 0) { ExtiCallbacks[2](); }
    EXTI->PR |= (0x01 << 2); 
}

void EXTI3_IRQHandler(void) {
    if (ExtiCallbacks[3] != 0) { ExtiCallbacks[3](); }
    EXTI->PR |= (0x01 << 3); 
}

void EXTI4_IRQHandler(void) {
    if (ExtiCallbacks[4] != 0) { ExtiCallbacks[4](); }
    EXTI->PR |= (0x01 << 4); 
}

void EXTI9_5_IRQHandler(void) {
    for (uint8 i = 5; i <= 9; i++) {
        if (EXTI->PR & (0x01 << i)) {
            if (ExtiCallbacks[i] != 0) { ExtiCallbacks[i](); }
            EXTI->PR |= (0x01 << i); 
        }
    }
}

void EXTI15_10_IRQHandler(void) {
    for (uint8 i = 10; i <= 15; i++) {
        if (EXTI->PR & (0x01 << i)) {
            if (ExtiCallbacks[i] != 0) { ExtiCallbacks[i](); }
            EXTI->PR |= (0x01 << i); 
        }
    }
}