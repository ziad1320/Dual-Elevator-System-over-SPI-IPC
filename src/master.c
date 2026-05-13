#include "Std_Types.h"
#include "Rcc.h"
#include "Gpio.h"
#include "Nvic.h"
#include "Usart.h"
#include "Dma.h"
#include "Timer.h"

// Telemetry string stored in RAM
char TelemetryBuffer[] = "Elevator System Active. Status: OK\r\n";

// The 500ms Timer Callback
void Telemetry_Send_Callback(void) {
    // Assuming Dma.h takes a 32-bit address for the memory source
    Dma1_Stream6_Send((uint32)TelemetryBuffer, sizeof(TelemetryBuffer) - 1);
}

int main(void) {
    // 1. Enable Peripheral Clocks
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_USART2);
    Rcc_Enable(RCC_DMA1);
    Rcc_Enable(RCC_TIM2);

    // 2. Configure PA8 (Motor LED) as Output
    Gpio_Init(GPIO_A, 8, GPIO_OUTPUT, GPIO_PUSH_PULL);

    // 3. Configure PA2 (UART TX) as Alternate Function (AF7)
    Gpio_Init(GPIO_A, 2, GPIO_AF, GPIO_PUSH_PULL);
    Gpio_SetAF(GPIO_A, 2, GPIO_AF7);

    // 4. Configure Port B Inputs with Pull-Down (Pins 0, 1, 2, 3, and 8)
    uint8 portB_pins[] = {0, 1, 2, 3, 8};
    for(int i = 0; i < 5; i++) {
        Gpio_Init(GPIO_B, portB_pins[i], GPIO_INPUT, GPIO_PULL_DOWN);
    }

    // 5. Configure Port C Inputs with Pull-Down (Pins 9 through 14)
    for(int i = 9; i <= 14; i++) {
        Gpio_Init(GPIO_C, i, GPIO_INPUT, GPIO_PULL_DOWN);
    }

    // 6. Peripheral Initialization
    Usart2_Init();
    Dma1_Stream6_Init(0x40004404UL); // USART2->DR Address

    // 7. Enable Interrupts
    Nvic_EnableIrq(28);            // TIM2 global interrupt is IRQ number 28
    __asm volatile("cpsie i");     // Inline assembly to enable global interrupts (Replaces __enable_irq())

    // 8. Start the 500ms Async Timer
    Timer_DelayMsAsync(TIMER_2, 500, Telemetry_Send_Callback);

    while (1) {
        // Put the CPU to sleep until the next interrupt fires
        __asm volatile("wfi");     // Inline assembly for Wait For Interrupt (Replaces __WFI())
    }
}

void _init(void) {
    /* Dummy implementation to satisfy libc initialization */
}