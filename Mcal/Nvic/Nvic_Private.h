/**
 * Nvic_Private.h
 *
 * Description: Private Register Definitions for the Cortex-M4 NVIC
 * Project    : Collaborative Dual-Elevator System
 */

#ifndef NVIC_PRIVATE_H
#define NVIC_PRIVATE_H

#include "Std_Types.h"

// The NVIC memory map perfectly structured as a struct
typedef struct {
    volatile uint32 ISER[8];       // Interrupt Set-Enable Registers (Offset 0x000)
    uint32 RESERVED0[24];
    volatile uint32 ICER[8];       // Interrupt Clear-Enable Registers (Offset 0x080)
    uint32 RESERVED1[24];
    volatile uint32 ISPR[8];       // Interrupt Set-Pending Registers (Offset 0x100)
    uint32 RESERVED2[24];
    volatile uint32 ICPR[8];       // Interrupt Clear-Pending Registers (Offset 0x180)
    uint32 RESERVED3[24];
    volatile uint32 IABR[8];       // Interrupt Active Bit Registers (Offset 0x200)
    uint32 RESERVED4[56];
    volatile uint8  IPR[240];      // Interrupt Priority Registers (Offset 0x300) - Byte accessible!
    uint32 RESERVED5[644];
    volatile uint32 STIR;          // Software Trigger Interrupt Register (Offset 0xE00)
} NvicType;

#define NVIC_BASE_ADDR 0xE000E100UL
#define NVIC ((NvicType*)NVIC_BASE_ADDR)

// System Control Block: Application Interrupt and Reset Control Register
// (Used to configure priority grouping)
#define SCB_AIRCR (*(volatile uint32*)0xE000ED0C)

#endif /* NVIC_PRIVATE_H */