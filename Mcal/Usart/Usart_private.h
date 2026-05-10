//
// Created by zramd on 10/05/2026.
//

#ifndef USART_PRIVATE_H
#define USART_PRIVATE_H

#include "Std_Types.h"

/* USART Register Map Struct */
typedef struct {
    volatile uint32 SR;    /* 0x00 - Status register */
    volatile uint32 DR;    /* 0x04 - Data register */
    volatile uint32 BRR;   /* 0x08 - Baud rate register */
    volatile uint32 CR1;   /* 0x0C - Control register 1 */
    volatile uint32 CR2;   /* 0x10 - Control register 2 */
    volatile uint32 CR3;   /* 0x14 - Control register 3 */
    volatile uint32 GTPR;  /* 0x18 - Guard time and prescaler register */
} UsartType;

/* USART2 Base Address Definition */
#define USART2_BASE_ADDR 0x40004400UL

/* Pointer to the USART2 peripheral mapped to the struct */
#define USART2_REG ((UsartType*)USART2_BASE_ADDR)

/* CR1 Bit Positions */
#define CR1_UE      13U  /* USART enable */
#define CR1_M       12U  /* Word length */
#define CR1_PCE     10U  /* Parity control enable */
#define CR1_TE      3U   /* Transmitter enable */
#define CR1_RE      2U   /* Receiver enable */
#define CR1_OVER8   15U  /* Oversampling mode */

/* SR Bit Positions */
#define SR_TXE      7U   /* Transmit data register empty */
#define SR_TC       6U   /* Transmission complete */
#define SR_RXNE     5U   /* Read data register not empty */

/* CR3 Bit Positions */
#define CR3_DMAT    7U   /* DMA enable transmitter */

#endif