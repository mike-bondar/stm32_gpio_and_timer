#ifndef USART_H
#define USART_H

#include <stdint.h>

#define _USART1_BASE 0x40013800
#define _USART2_BASE 0x40004400
#define USART1 ((USART_Type *) _USART1_BASE)
#define USART2 ((USART_Type *) _USART2_BASE)

typedef struct {
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
  volatile uint32_t BRR;
  volatile uint32_t GTPR;
  volatile uint32_t RTOR;
  volatile uint32_t RQR;
  volatile uint32_t ISR;
  volatile uint32_t ICR;
  volatile uint32_t RDR;
  volatile uint32_t TDR;
} USART_Type;

USART_Type *
usart1_start(uint32_t clock_speed);

void
usart_write_new_line(USART_Type * usart);

char
usart_receive(USART_Type * usart);

void
usart_write_str(USART_Type * usart, char * data);

void
usart_write_byte(USART_Type * usart, char data);

void
usart_write_binary(USART_Type * usart, uint32_t number);

#endif
