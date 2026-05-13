/**
 * Dma_Private.h
 *
 * Description: Private Register Definitions for STM32F401xE DMA1
 * Project    : Collaborative Dual-Elevator System
 * Author     : ziad
 */

#ifndef DMA_PRIVATE_H
#define DMA_PRIVATE_H

#include "Std_Types.h"

/* Define the 6 registers that make up a single DMA Stream.
 * These match the offsets (0x00, 0x04, 0x08, 0x0C, 0x10, 0x14)
 * for every stream exactly.
 */
typedef struct {
    volatile uint32 CR;      /* Configuration Register */
    volatile uint32 NDTR;    /* Number of Data Register */
    volatile uint32 PAR;     /* Peripheral Address Register */
    volatile uint32 M0AR;    /* Memory 0 Address Register */
    volatile uint32 M1AR;    /* Memory 1 Address Register */
    volatile uint32 FCR;     /* FIFO Control Register */
} DmaStreamType;

/* Define the main DMA Controller Memory Map.
 * Global status registers first, followed by an array of 8 Streams.
 */
typedef struct {
    volatile uint32 LISR;    /* 0x00 - Low Interrupt Status */
    volatile uint32 HISR;    /* 0x04 - High Interrupt Status */
    volatile uint32 LIFCR;   /* 0x08 - Low Interrupt Flag Clear */
    volatile uint32 HIFCR;   /* 0x0C - High Interrupt Flag Clear */
    DmaStreamType STREAM[8]; /* 0x10 to 0xCC - Array of 8 Streams */
} DmaType;

/* Base Address for DMA2 */
#define DMA2_BASE_ADDR 0x40026400UL

/* The Pointer Macro (The Magic Key) */
#define DMA2_REG ((DmaType*)DMA2_BASE_ADDR)


/* ========================================================================= */
/* DMA STREAM CONTROL REGISTER (CR) BITS                                     */
/* ========================================================================= */
#define DMA_CR_EN       0U   /* Stream Enable */
#define DMA_CR_TCIE     4U   /* Transfer Complete Interrupt Enable */
#define DMA_CR_DIR      6U   /* Data Transfer Direction (6-7) */
#define DMA_CR_MINC     10U  /* Memory Increment Mode */
#define DMA_CR_CHSEL    25U  /* Channel Selection (25-27) */

/* DMA Direction Options */
#define DMA_DIR_MEM_TO_PERIPH  1U

/* ========================================================================= */
/* HIFCR REGISTER BITS (For clearing Stream 7 flags)                         */
/* ========================================================================= */
#define DMA_HIFCR_CTCIF7  27U /* Clear Transfer Complete Interrupt Flag for Stream 7 */

#endif /* DMA_PRIVATE_H */