/**
 * Motor.h
 *
 * Description: PWM Motor Driver using Timer APIs
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "../Mcal/Lib/Std_Types.h"

/* Motor Speed Definitions (Duty Cycle Percentages) */
#define MOTOR_REST         0U
#define MOTOR_SLOW_SPEED   20U
#define MOTOR_HIGH_SPEED   100U

/* ========================================================================= */
/* FUNCTION PROTOTYPES                                                       */
/* ========================================================================= */

void Motor_Init(void);
void Motor_SetSpeed(uint8 SpeedPercentage);

#endif /* MOTOR_H */
