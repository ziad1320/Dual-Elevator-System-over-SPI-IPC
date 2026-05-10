#include "stm32f4xx.h"

int main(void) {
    // 1. Enable Clocks for GPIOA, GPIOB, and GPIOC
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

    // 2. Configure PA8 (Motor LED) and PA2 (UART TX) as Outputs
    GPIOA->MODER |= (1 << (8 * 2)) | (1 << (2 * 2)); 

    // 3. Configure Port B (0-3, 8) as Inputs with Internal Pull-Down Resistors
    for(int i = 0; i <= 8; i++) {
        GPIOB->PUPDR |= (2 << (i * 2)); 
    }
    
    // 4. Configure Port C (9-14) as Inputs with Internal Pull-Down Resistors
    for(int i = 9; i <= 14; i++) {
        GPIOC->PUPDR |= (2 << (i * 2)); 
    }

    while (1) {
        // Read Port B (Cabin/Emergency)
        uint32_t cabinInput = GPIOB->IDR & 0x010F; 
        
        // Read Port C (Hallway Calls) - Now looking at pins 9 through 14
        uint32_t hallwayInput = GPIOC->IDR & 0x7E00;

        if (cabinInput || hallwayInput) {
            GPIOA->BSRR = (1 << 8) | (1 << 2); // LED ON + UART Pulse
        } else {
            GPIOA->BSRR = (1 << 24) | (1 << 18); // Everything OFF
        }
    }
}