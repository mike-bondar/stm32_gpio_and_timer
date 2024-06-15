#include "drivers.h"
#include <stdbool.h>
#include "usart.h"

static volatile uint8_t usart1_buffer[255] = {0};
static volatile uint8_t usart1_buffer_position = 0;

void
USART1_IRQHandler(void)
{
  char byte = usart_receive(USART1);

  if (usart1_buffer_position < 255 && byte)
  {
    usart1_buffer[usart1_buffer_position] = byte;
    usart1_buffer_position++;
  }
}

static TIM2_Type * 
timer_start(void)
{
  // TIM2 timer clock enable
  RCC_APB1ENR1 |= 1 << TIM2EN;

  TIM2_Type * timer = TIM2;
  timer->CR1 |= 1; //Enable timer

  return timer;
}

int
main(void)
{
  USART_Type * usart = usart1_start(4000000);

  usart_write_str(usart, "USART1 Started");
  usart_write_new_line(usart);

  TIM2_Type * timer = timer_start();

  uint8_t read_pos = 0;

  while(1)
  {
    if (read_pos < usart1_buffer_position)
    {
      uint8_t usart_byte = usart1_buffer[read_pos];
      usart_write_byte(usart, usart_byte);
      read_pos++;

      usart_write_binary(usart, timer->CNT);
      usart_write_new_line(usart);
    }

    if (read_pos > 0)
    {
      if (read_pos == usart1_buffer_position)
      {
        usart1_buffer_position = 0;
        read_pos = 0;
      }
    }

  }

  return 1;
}
