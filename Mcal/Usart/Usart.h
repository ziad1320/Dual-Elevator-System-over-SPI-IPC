//
// Created by Khalaf on 29/04/2026.
//

#ifndef USART_H
#define USART_H
#include "Std_Types.h"


void Usart1_Init(void);

uint8 Usart1_TransmitByte(uint8 Byte);

uint8 Usart1_RecieveByte(void);

void Usart1_TransmitString(const char* Str);
void Usart1_TransmitString_Async(const char* Str, uint16 Length);


#endif //USART_H
