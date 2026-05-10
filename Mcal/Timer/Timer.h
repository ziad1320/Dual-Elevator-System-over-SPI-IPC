/**
 * Timer.h
 *
 * Description: Public API for Timer and PWM functionality
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef TIMER_H
#define TIMER_H

#include "Std_Types.h"

#define NUM_OF_TIMERS 5U

/* Timer IDs */
#define TIMER_1 1U // Advanced (Use for PWM)
#define TIMER_2 2U // General (Use for Base FSM timing)
#define TIMER_3 3U
#define TIMER_4 4U
#define TIMER_5 5U

/* PWM Channels */
#define CH1 1U
#define CH2 2U
#define CH3 3U
#define CH4 4U

// Callback function pointer type for async delays
typedef void (*Timer_Callback)(void);

/* ========================================================================= */
/* FUNCTION PROTOTYPES                                                       */
/* ========================================================================= */

// General Timer Functions
void Timer_Init(uint8 TimerId, uint16 Prescaler, uint16 AutoReload);
void Timer_Start(uint8 TimerId);
void Timer_Stop(uint8 TimerId);

// Delay Functions (Non-blocking required for Elevator FSM)
void Timer_DelayMs(uint8 TimerId, uint32 DelayMs);
void Timer_DelayMsAsync(uint8 TimerId, uint32 DelayMs, Timer_Callback Callback);

// PWM Functions (Required for Motor Simulation)
void Timer_PwmInit(uint8 TimerId, uint8 Channel);
void Timer_PwmSetDuty(uint8 TimerId, uint8 Channel, uint8 DutyPercent);

#endif /* TIMER_H */