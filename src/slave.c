#include "stm32f4xx.h"

/**
 * Single Elevator Car Controller for STM32F4
 * 
 * Hardware Mapping:
 * - Port A:
 *   - PA4 - PA7: Floor Sensors (F0 - F3), Inputs with Pull-Down
 *   - PA8: Motor UP LED, Output
 *   - PA9: Motor DOWN LED, Output
 * - Port B:
 *   - PB0 - PB8: Cabin/Hallway Buttons, Inputs with Pull-Down
 * - Port C:
 *   - PC0 - PC5: Hallway Call Buttons, Inputs with Pull-Down
 */

// Define floor sensor pins on Port A
#define SENSOR_F0 (1 << 4)
#define SENSOR_F1 (1 << 5)
#define SENSOR_F2 (1 << 6)
#define SENSOR_F3 (1 << 7)

// Define motor control pins on Port A
#define MOTOR_UP   (1 << 8)
#define MOTOR_DOWN (1 << 9)

int main(void) {
  // 1. Enable Clocks for GPIOA, GPIOB, and GPIOC
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

  // 2. Configure Port A Pins
  // PA4-PA7: Inputs (Sensors), PA8-PA9: Outputs (Motor)
  // Clear MODER bits for pins 4-9
  GPIOA->MODER &= ~((0x3 << (4 * 2)) | (0x3 << (5 * 2)) | (0x3 << (6 * 2)) | (0x3 << (7 * 2)) |
                    (0x3 << (8 * 2)) | (0x3 << (9 * 2)));
  // Set PA8, PA9 to Output (01)
  GPIOA->MODER |= (0x1 << (8 * 2)) | (0x1 << (9 * 2));

  // PUPDR: Set PA4-PA7 to Pull-down (10)
  GPIOA->PUPDR &= ~((0x3 << (4 * 2)) | (0x3 << (5 * 2)) | (0x3 << (6 * 2)) | (0x3 << (7 * 2)));
  GPIOA->PUPDR |= (0x2 << (4 * 2)) | (0x2 << (5 * 2)) | (0x2 << (6 * 2)) | (0x2 << (7 * 2));

  // 3. Configure Port B (Pins 0-8) as Inputs with Pull-Down
  GPIOB->MODER &= ~(0x3FFFF); // Pins 0-8 to Input (00)
  for (int i = 0; i <= 8; i++) {
    GPIOB->PUPDR &= ~(0x3 << (i * 2));
    GPIOB->PUPDR |= (0x2 << (i * 2)); // Pull-down (10)
  }

  // 4. Configure Port C (Pins 0-5) as Inputs with Pull-Down
  GPIOC->MODER &= ~(0xFFF); // Pins 0-5 to Input (00)
  for (int i = 0; i <= 5; i++) {
    GPIOC->PUPDR &= ~(0x3 << (i * 2));
    GPIOC->PUPDR |= (0x2 << (i * 2)); // Pull-down (10)
  }

  int targetFloor = -1;
  int currentFloor = -1;

  while (1) {
    // Read sensors to determine current floor
    uint32_t sensors = GPIOA->IDR & (SENSOR_F0 | SENSOR_F1 | SENSOR_F2 | SENSOR_F3);
    
    if (sensors & SENSOR_F0) currentFloor = 0;
    else if (sensors & SENSOR_F1) currentFloor = 1;
    else if (sensors & SENSOR_F2) currentFloor = 2;
    else if (sensors & SENSOR_F3) currentFloor = 3;

    // Read buttons (Cabin on B, Hallway on C)
    uint32_t buttonsB = GPIOB->IDR & 0x1FF;
    uint32_t buttonsC = GPIOC->IDR & 0x3F;

    // Simple priority: Floor 0 > 1 > 2 > 3 if multiple buttons are pressed
    // Mapping: B0-B3 or C0-C3 are mapped to Floors 0-3
    if ((buttonsB & (1 << 0)) || (buttonsC & (1 << 0))) targetFloor = 0;
    else if ((buttonsB & (1 << 1)) || (buttonsC & (1 << 1))) targetFloor = 1;
    else if ((buttonsB & (1 << 2)) || (buttonsC & (1 << 2))) targetFloor = 2;
    else if ((buttonsB & (1 << 3)) || (buttonsC & (1 << 3))) targetFloor = 3;

    // Movement Logic
    if (targetFloor != -1) {
      // Check if we arrived at the target floor sensor
      uint32_t targetSensor = 0;
      switch(targetFloor) {
        case 0: targetSensor = SENSOR_F0; break;
        case 1: targetSensor = SENSOR_F1; break;
        case 2: targetSensor = SENSOR_F2; break;
        case 3: targetSensor = SENSOR_F3; break;
      }

      if (sensors & targetSensor) {
        // Stop movement: Reset PA8 and PA9
        GPIOA->BSRR = (MOTOR_UP << 16) | (MOTOR_DOWN << 16);
        targetFloor = -1;
      } else if (currentFloor != -1) {
        // Decide direction based on current (last known) floor
        if (targetFloor > currentFloor) {
          GPIOA->BSRR = MOTOR_UP;             // Set UP
          GPIOA->BSRR = (MOTOR_DOWN << 16);   // Reset DOWN
        } else if (targetFloor < currentFloor) {
          GPIOA->BSRR = MOTOR_DOWN;           // Set DOWN
          GPIOA->BSRR = (MOTOR_UP << 16);     // Reset UP
        }
      }
    }
  }
}

void _init(void) {
  /* Dummy implementation to satisfy libc initialization */
}