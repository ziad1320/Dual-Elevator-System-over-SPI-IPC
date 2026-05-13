//
// Created by Khalaf on 29/04/2026.
//
#include "stm32f401xe.h"
#include "Usart.h"
#include "BIT_MATH.h"
#include "../GPIO/GPIO.h"
#include "../RCC/RCC.h"
#include "../DMA/DMA.h"



void Usart1_Init(void) {
    uint8 usart1_bus_id = 3;
    uint8 usart1_bit_pos = 4;

     // Calculate final ID: (BusID * 32) + BitPos
    uint8 usart1_peripheral_id = (usart1_bus_id * 32) + usart1_bit_pos;

    Rcc_Enable(usart1_peripheral_id);
        // 1 & 2. Initialize both pins using Gpio API
    Gpio_Init(GPIO_A, 9, GPIO_AF, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_A, 10, GPIO_AF, GPIO_PUSH_PULL);

    Gpio_SetAF(GPIO_A, 9, GPIO_AF7);
    Gpio_SetAF(GPIO_A, 10, GPIO_AF7);

    // 3. Configure USART1 Frame (8 Data bits, 1 Stop bit, No Parity)
    CLEAR_BIT(USART1->CR1, USART_CR1_M_Pos); // 8-bit word length


    USART1->CR2 &= ~(USART_CR2_STOP_Msk); // 1-stop bit at the end
    CLEAR_BIT(USART1->CR1, USART_CR1_OVER8_Pos);  // 16 over sampling

    // 4. Hardcoded Baud Rate (9600 based on your clock tree setup)
    USART1->BRR = 0x683;

    // 5. Enable TX, RX, and the USART peripheral
    SET_BIT(USART1->CR1, USART_CR1_TE_Pos);
    SET_BIT(USART1->CR1, USART_CR1_RE_Pos);
    SET_BIT(USART1->CR1, USART_CR1_UE_Pos);

    // 6. Initialize DMA for USART1 TX and enable DMAT
    Dma2_Stream7_Init((uint32)&USART1->DR);
    USART1->CR3 |= USART_CR3_DMAT_Msk;
}

uint8 Usart1_TransmitByte(uint8 Byte) {
    if (USART1->SR & USART_SR_TXE_Msk) {
        USART1->DR = Byte;
        while (!(USART1->SR & USART_SR_TC_Msk));
        USART1->SR &= ~(USART_SR_TC_Msk); // Clearing TC bit
        return OK;
    }
    return NOK;
}

void Usart1_TransmitString(const char *Str) {
    uint32 i = 0;
    uint8 transmitResult = -1;
    while (Str[i] != '\0') {
        transmitResult = Usart1_TransmitByte((uint8)Str[i]);
        if (transmitResult == OK) {
            i++;
        }
    }
}

void Usart1_TransmitString_Async(const char *Str, uint16 Length) {
    Dma2_Stream7_Send((uint32)Str, Length);
}



uint8 Usart1_RecieveByte(void) {
    while (!(USART1->SR & USART_SR_RXNE_Msk));
    return USART1->DR;
}

