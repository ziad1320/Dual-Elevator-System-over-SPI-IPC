/**
 * Rcc_Private.h
 *
 * Description: Private Register Definitions for STM32F401xE Reset and Clock
 * Control Project    : Collaborative Dual-Elevator System
 */

#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include "Std_Types.h"

// Note: Assuming REG32 macro is defined in your Utils.h
// #define REG32(ADDR) (*((volatile uint32 *)(ADDR)))
#include "Utils.h"

#define RCC_BASE_ADDR 0x40023800UL

// Clock Control & Configuration
#define RCC_CR REG32(RCC_BASE_ADDR + 0x00UL)
#define RCC_PLLCFGR REG32(RCC_BASE_ADDR + 0x04UL)
#define RCC_CFGR REG32(RCC_BASE_ADDR + 0x08UL)

// Peripheral Clock Enable Registers
#define RCC_AHB1ENR REG32(RCC_BASE_ADDR + 0x30UL)
#define RCC_AHB2ENR REG32(RCC_BASE_ADDR + 0x34UL)
#define RCC_APB1ENR REG32(RCC_BASE_ADDR + 0x40UL)
#define RCC_APB2ENR REG32(RCC_BASE_ADDR + 0x44UL)

// Peripheral Reset Registers
#define RCC_AHB1RSTR REG32(RCC_BASE_ADDR + 0x10UL)
#define RCC_AHB2RSTR REG32(RCC_BASE_ADDR + 0x14UL)
#define RCC_APB1RSTR REG32(RCC_BASE_ADDR + 0x20UL)
#define RCC_APB2RSTR REG32(RCC_BASE_ADDR + 0x24UL)

#endif /* RCC_PRIVATE_H */