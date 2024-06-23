#include "usart.h"
#include "cortex.h"
#include "drivers.h"
//
// 1. Program M bits in USART_CR1
// 2. Select BAUD rate using USART_BRR
// 3. Program the number of stop bits in USART_CR2
// 4. Enable USART by writing the UE bit in USART_CR1 register to 1
// 5. Set the TE bit in USART_CR1 to send an idle frame as first transmission
// 6. Wait till TXE bit is set and write data to USART_TDR
// 7. Once all data is written, wait till TC=1 and then disable it
static void
usart_start(uint32_t clock_speed, USART_Type * usart)
{
  // Ensure USART is turned off
  usart->CR1 &= ~1;

  // Program M bits. 00 - 8 data bits
  usart->CR1 &= ~(1 << 28); // M1
  usart->CR1 &= ~(1 << 12); // M0

  // Setup oversampling
  usart->CR1 &= ~(1 << 15); // Set OVER8 to 0, oversampling by 16

  usart->BRR = 35; // To get a baud rate of 115200
  
  // Set stop bits to 1 bit
  usart->CR2 &= ~(0b00 << 12);

  // Enable transmitter
  usart->CR1 |= 1 << 3;

  // Enable receiver
  usart->CR1 |= 1 << 2;

  // Enable USART
  usart->CR1 |= 1;

  // Enable receiver interrupt
  usart->CR1 |= 1 << 5;
  usart->CR3 |= 1;

  // 37 comes from the NVIC list vector
  // 31 and >> 5 is dictated by the ISER reprenentation
  // Enable global interrupt
}


USART_Type *
usart1_start(uint32_t clock_speed)
{
  RCC_APB2ENR |= 1 << USART1EN;
  RCC_AHB2ENR |= 1 << GPIOAEN;

  // Configure A10 and A9 as alternative functions
  GPIOA->MODER &= ~(0b11 << 20);
  GPIOA->MODER &= ~(0b11 << 18);
  GPIOA->MODER |= 0b10 << 20;
  GPIOA->MODER |= 0b10 << 18;
  GPIOA->AFRH |= 0b0111 << 4;
  GPIOA->AFRH |= 0b0111 << 8;

  USART_Type * usart = USART1;

  usart_start(clock_speed, usart);

  NVIC->ISER[37 >> 5] |= 1 << (37 & 0x1F);

  return usart;
}

void
usart_write_byte(USART_Type * usart, char byte)
{
  while(!(usart->ISR & (1 << 7)));

  usart->TDR = byte;
}

void
usart_write_str(USART_Type * usart, char * data)
{
  uint32_t i = 0;

  while(data[i])
  {
    usart_write_byte(usart, data[i]); 
    data++;
  }
}

void
usart_write_new_line(USART_Type * usart)
{
  usart_write_str(usart, "\n");
}

char
usart_receive(USART_Type * usart)
{
  if (usart->ISR & (1 << 5))
  {
    return usart->RDR;
  }

  return 0;
}

void
usart_write_binary(USART_Type * usart, uint32_t number)
{
  usart_write_str(usart, "0b");
  uint8_t bit = 31;

  for (uint8_t section = 0; section < 8; section++)
  {
    for (uint8_t section_bit = 0; section_bit < 4; section_bit++)
    {
      if (number & (1 << bit))
      {
        usart_write_byte(usart, '1');
      }
      else
      {
        usart_write_byte(usart, '0');
      }
      bit--;
    }
    usart_write_byte(usart, ' ');
  }
}
