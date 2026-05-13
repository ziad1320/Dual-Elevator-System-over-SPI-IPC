/**
 * Timer.c
 *
 * Description: Implementation of STM32F401xE Timer and PWM driver
 * Project    : Collaborative Dual-Elevator System
 */

#include "Timer.h"
#include "Timer_Private.h"
#include "Nvic.h"
#include "Bit_Math.h"

// Address mapping (1-indexed to match TimerId)
static const uint32 timer_base_addresses[NUM_OF_TIMERS] = {
    TIM1_BASE_ADDR, 
    TIM2_BASE_ADDR, 
    TIM3_BASE_ADDR, 
    TIM4_BASE_ADDR, 
    TIM5_BASE_ADDR
};

#define TIMER_GET_PERIPHERAL(TimerId) ((TimerType*)timer_base_addresses[TimerId - 1])

/* NVIC IRQ numbers for STM32F401: TIM1_UP=25, TIM2=28, TIM3=29, TIM4=30, TIM5=50 */
static const uint8 Timer_NvicIrq[NUM_OF_TIMERS] = {25, 28, 29, 30, 50};

/* Array to hold async callbacks */
static Timer_Callback Timer_Callbacks[NUM_OF_TIMERS] = {0};

/* ========================================================================= */
/* GENERAL TIMER FUNCTIONS                                                   */
/* ========================================================================= */

void Timer_Init(uint8 TimerId, uint16 Prescaler, uint16 AutoReload) {
    TimerType* timer = TIMER_GET_PERIPHERAL(TimerId);
    timer->CR1 = 0; // Reset control register
    timer->PSC = Prescaler;
    timer->ARR = AutoReload;
    timer->CNT = 0; 
    
    SET_BIT(timer->EGR, EGR_UG); // Force update generation
    timer->SR = 0;               // Clear flag
}

void Timer_Start(uint8 TimerId) {
    SET_BIT(TIMER_GET_PERIPHERAL(TimerId)->CR1, CR1_CEN);
}

void Timer_Stop(uint8 TimerId) {
    CLEAR_BIT(TIMER_GET_PERIPHERAL(TimerId)->CR1, CR1_CEN);
}

/* ========================================================================= */
/* PWM MOTOR FUNCTIONS                                                       */
/* ========================================================================= */

void Timer_PwmInit(uint8 TimerId, uint8 Channel) {
    TimerType* timer = TIMER_GET_PERIPHERAL(TimerId);

    // 1. Configure CCMRx for PWM Mode 1 and enable Preload
    if (Channel == CH1) {
        timer->CCMR1 &= ~(0xFF); // Clear CH1 config
        timer->CCMR1 |= CCMR_OC_PWM1_PRELOAD;
        SET_BIT(timer->CCER, 0); // Enable CH1 output (CC1E)
    }

    // 2. CRITICAL FOR TIM1: Main Output Enable (MOE) in BDTR register
    // Advanced timers will NOT output physical signals without this bit set!
    if (TimerId == TIMER_1) {
        SET_BIT(timer->BDTR, BDTR_MOE);
    }
}

void Timer_PwmSetDuty(uint8 TimerId, uint8 Channel, uint8 DutyPercent) {
    TimerType* timer = TIMER_GET_PERIPHERAL(TimerId);
    
    // Safety clamp
    if (DutyPercent > 100) DutyPercent = 100;

    // Calculate CCR value dynamically based on current Auto-Reload Register
    uint32 currentArr = timer->ARR;
    uint32 ccrValue = (currentArr * DutyPercent) / 100;

    if (Channel == CH1) {
        timer->CCR1 = ccrValue;
    }
}

/* ========================================================================= */
/* DELAY & ASYNC FUNCTIONS                                                   */
/* ========================================================================= */

void Timer_DelayMs(uint8 TimerId, uint32 DelayMs) {
    TimerType* timer = TIMER_GET_PERIPHERAL(TimerId);
    timer->CR1 = 0; 
    
    // 16 MHz Clock -> 15999 PSC = 1 ms per tick
    timer->PSC = 15999U; 
    timer->ARR = (uint16)(DelayMs - 1); 
    timer->CNT = 0;

    SET_BIT(timer->EGR, EGR_UG); 
    timer->SR = 0; 

    SET_BIT(timer->CR1, CR1_OPM); // One-pulse mode
    SET_BIT(timer->CR1, CR1_CEN); // Start counting

    while (!READ_BIT(timer->SR, SR_UIF)); // Blocking poll

    timer->SR = 0; 
    CLEAR_BIT(timer->CR1, CR1_CEN); 
}

void Timer_DelayMsAsync(uint8 TimerId, uint32 DelayMs, Timer_Callback Callback) {
    uint8 index = TimerId - 1;
    TimerType* timer = TIMER_GET_PERIPHERAL(TimerId);
    uint8 irqNum = Timer_NvicIrq[index];

    Timer_Callbacks[index] = Callback;

    timer->CR1 = 0; 
    timer->PSC = 15999U;
    timer->ARR = (uint16)(DelayMs - 1);
    timer->CNT = 0;

    SET_BIT(timer->EGR, EGR_UG); 
    timer->SR = 0; 

    SET_BIT(timer->CR1, CR1_OPM); // One-pulse mode
    SET_BIT(timer->DIER, DIER_UIE); // Enable update interrupt 
    
    Nvic_EnableIrq(irqNum); // Enable at NVIC level

    SET_BIT(timer->CR1, CR1_CEN); // Start counting
}

/* ========================================================================= */
/* INTERRUPT SERVICE ROUTINES                                                */
/* ========================================================================= */

static void Timer_HandleIrq(uint8 index) {
    TimerType* timer = (TimerType*)timer_base_addresses[index];

    if (READ_BIT(timer->SR, SR_UIF)) {
        timer->SR = 0; // Clear flag
        CLEAR_BIT(timer->DIER, DIER_UIE); // Disable further IRQs
        CLEAR_BIT(timer->CR1, CR1_CEN); // Stop counter

        if (Timer_Callbacks[index] != 0) {
            Timer_Callbacks[index]();
        }
    }
}

void TIM1_UP_TIM10_IRQHandler(void) { Timer_HandleIrq(0); }
void TIM2_IRQHandler(void)          { Timer_HandleIrq(1); }
void TIM3_IRQHandler(void)          { Timer_HandleIrq(2); }
void TIM4_IRQHandler(void)          { Timer_HandleIrq(3); }
void TIM5_IRQHandler(void)          { Timer_HandleIrq(4); }