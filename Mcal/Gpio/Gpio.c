/**
 * Gpio.c
 *
 * Description: Implementation of STM32F401xE GPIO Driver
 * Project    : Collaborative Dual-Elevator System
 */

#include "Gpio.h"
#include "Gpio_Private.h"

// Address mapping array for 'A' through 'D'
static const uint32 addressMap[4] = {
    GPIOA_BASE_ADDR, 
    GPIOB_BASE_ADDR, 
    GPIOC_BASE_ADDR, 
    GPIOD_BASE_ADDR
};

void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];

    // 1. Configure Pin Mode (Input, Output, AF, Analog)
    gpioDevice->GPIO_MODER &= ~(0x03 << (PinNumber * 2));
    gpioDevice->GPIO_MODER |=  (PinMode << (PinNumber * 2));

    // 2. Configure Default State based on Mode
    if (PinMode == GPIO_INPUT) {
        // Apply Pull-up / Pull-down Resistors
        gpioDevice->GPIO_PUPDR &= ~(0x03 << (PinNumber * 2));
        gpioDevice->GPIO_PUPDR |=  (DefaultState << (PinNumber * 2));
    } 
    else if (PinMode == GPIO_OUTPUT || PinMode == GPIO_AF) {
        // Apply Output Type (Push-Pull or Open-Drain)
        gpioDevice->GPIO_OTYPER &= ~(0x1 << PinNumber);
        gpioDevice->GPIO_OTYPER |=  (DefaultState << PinNumber);
        
        // Default to high speed for SPI/PWM stability
        gpioDevice->GPIO_OSPEEDR |= (0x03 << (PinNumber * 2));
    }
}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    uint8 status = NOK;
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];

    // Check if the pin is NOT configured as a pure input before writing
    if (((gpioDevice->GPIO_MODER >> (PinNumber * 2)) & 0x03) != GPIO_INPUT) {
        // Using BSRR is highly recommended for atomic bit manipulation, 
        // but keeping your ODR logic to match your style.
        if (Data == HIGH) {
            gpioDevice->GPIO_ODR |= (1 << PinNumber);
        } else {
            gpioDevice->GPIO_ODR &= ~(1 << PinNumber);
        }
        status = OK;
    }
    return status;
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber) {
    uint8 data = LOW;
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];
    
    // Read the specific bit from Input Data Register
    data = (gpioDevice->GPIO_IDR >> PinNumber) & 0x01;
    
    return data;
}

void Gpio_SetAF(uint8 PortName, uint8 PinNumber, uint8 AF) {
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];

    if (PinNumber < 8) {
        // Pins 0-7 use Alternate Function Low Register (AFRL)
        gpioDevice->GPIO_AFRL &= ~((uint32)0x0F << (PinNumber * 4));
        gpioDevice->GPIO_AFRL |=  ((uint32)AF   << (PinNumber * 4));
    } else {
        // Pins 8-15 use Alternate Function High Register (AFRH)
        uint8 pos = PinNumber - 8;
        gpioDevice->GPIO_AFRH &= ~((uint32)0x0F << (pos * 4));
        gpioDevice->GPIO_AFRH |=  ((uint32)AF   << (pos * 4));
    }
}