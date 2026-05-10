/**
 * Exti_Private.h
 *
 * Description: Private Register Definitions for STM32F401xE EXTI, SYSCFG, and NVIC
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#include "Std_Types.h"

typedef struct {
    volatile uint32 IMR;
    volatile uint32 EMR;
    volatile uint32 RTSR;
    volatile uint32 FTSR;
    volatile uint32 SWIER;
    volatile uint32 PR;
} ExtiType;

typedef struct {
    volatile uint32 NVIC_ISER[8];
    uint32 _r[24];
    volatile uint32 NVIC_ICER[8];
} NvicType;

typedef struct {
    volatile uint32 MEMRMP;
    volatile uint32 PMC;
    volatile uint32 EXTICR[4];
    uint32 _r[2];
    volatile uint32 CMPCR;
} SyscfgType;

#define EXTI          ((ExtiType*)0x40013C00UL)
#define NVIC          ((NvicType*)0xE000E100UL)
#define SYSCFG        ((SyscfgType*)0x40013800UL)

#endif /* EXTI_PRIVATE_H */