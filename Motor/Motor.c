/**
 * Motor.c
 *
 * Description: PWM Motor Driver using Timer 2 CH1 (PA5)
 * Project    : Collaborative Dual-Elevator System
 */

#include "Motor.h"
#include "../Mcal/Timer/Timer.h"
#include "../Mcal/Gpio/Gpio.h"
#include "../Mcal/RCC/RCC.h"

void Motor_Init(void) {
    // 1. Enable GPIOA Clock (AHB1ENR bit 0 = Peripheral ID 0)
    Rcc_Enable(0);

    // 2. Configure PA5 as Alternate Function
    // Using GPIO_A (Port A), Pin 5. Mode = AF, Default State = Push-Pull
    Gpio_Init(GPIO_A, 5, GPIO_AF, GPIO_PUSH_PULL);
    
    // Set Alternate Function 1 (AF1) for PA5 -> TIM2_CH1
    Gpio_SetAF(GPIO_A, 5, GPIO_AF1);

    // 3. Initialize Timer 2 Channel 1 for PWM Generation
    // Clock = 16MHz. Prescaler = 15 -> 1MHz (1us tick).
    // Period = 100. So 100 ticks = 100us -> 10kHz frequency.
    Timer_ConfigChannel(TIM_INSTANCE_2, TIM_CHANNEL_1, TIM_PRESCALER_1US_TICK, TIM_OC_PWM1_PRELOAD, 100);

    // Initialize to rest (0% duty cycle)
    Motor_SetSpeed(MOTOR_REST);
}

void Motor_SetSpeed(uint8 SpeedPercentage) {
    if (SpeedPercentage > 100) {
        SpeedPercentage = 100;
    }
    
    // Set Compare Value (CCR). 
    // Since our ARR (Period) is 100, CCR = DutyCycle% matches 1:1
    Timer_SetCompareValue(TIM_INSTANCE_2, TIM_CHANNEL_1, SpeedPercentage);
}
