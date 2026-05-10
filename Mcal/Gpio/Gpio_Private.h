/**
 * Gpio_Private.h
 *
 * Description: Private Register Definitions for STM32F401xE GPIO
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include "Std_Types.h"

// Base addresses for STM32F401xE AHB1 Peripheral Bus
#define GPIOA_BASE_ADDR    0x40020000UL
#define GPIOB_BASE_ADDR    0x40020400UL
#define GPIOC_BASE_ADDR    0x40020800UL
#define GPIOD_BASE_ADDR    0x40020C00UL
#define GPIOE_BASE_ADDR    0x40021000UL
#define GPIOH_BASE_ADDR    0x40021C00UL

// Register map for a GPIO Port
typedef struct {
    volatile uint32 GPIO_MODER;    // Offset 0x00
    volatile uint32 GPIO_OTYPER;   // Offset 0x04
    volatile uint32 GPIO_OSPEEDR;  // Offset 0x08
    volatile uint32 GPIO_PUPDR;    // Offset 0x0C
    volatile uint32 GPIO_IDR;      // Offset 0x10
    volatile uint32 GPIO_ODR;      // Offset 0x14
    volatile uint32 GPIO_BSRR;     // Offset 0x18
    volatile uint32 GPIO_LCKR;     // Offset 0x1C
    volatile uint32 GPIO_AFRL;     // Offset 0x20
    volatile uint32 GPIO_AFRH;     // Offset 0x24
} GpioType;

#endif /* GPIO_PRIVATE_H */