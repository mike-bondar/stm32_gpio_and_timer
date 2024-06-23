#include "drivers.h"
#include <stdbool.h>
#include "usart.h"
#include "cortex.h"

// The clock is supposed to run at 48Mhz
uint32_t
clock_setup(void)
{
  // Set voltage range to Range 1 to get 48Mhz
  PWR_CR1 |= 0b01 << VOS;

  RCC_CR |= 1 << MSION;
  RCC_CR |= 0b1011 << MSIRANGE;
  RCC_CR |= 1 << MSIPLLEN;

  while (!(RCC_CR & (1 << MSIRDY)));

  RCC_CFGR &= ~(0b11 << SW);

  return 48000000;
}

static volatile uint8_t usart1_buffer[255] = {0};
static volatile uint8_t usart1_buffer_position = 0;

void
USART1_IRQHandler(void)
{
  char byte = usart_receive(USART1);

  if(usart1_buffer_position < 255 && byte)
  {
    usart1_buffer[usart1_buffer_position] = byte;
    usart1_buffer_position++;
  }
}

static volatile bool button_is_pressed = false;

void
EXTI0_IRQHandler(void)
{
  button_is_pressed = true;
  EXTI->PR1 |= 1;
}

static TIM_2_3_Type * 
timer2_setup(void)
{
  // TIM2 timer clock enable
  RCC_APB1ENR1 |= 1 << TIM2EN;
  NVIC->ISER[28 >> 5] |= 1 << (28 & 0x1F); // Enable TIM2 interrupts

  TIM_2_3_Type * timer = TIM2;
  timer->PSC = 48000; //To get 1Khz on timer and have access to milliseconds

  return timer;
}

volatile bool timer_scheduled = false;

void
TIM2_IRQHandler(void)
{
  if (TIM2->SR & (1 << CC1IF))
  {
    timer_scheduled = false;
  }

  TIM2->SR = 0;
}

void
timer_schedule(TIM_2_3_Type * timer, uint32_t delay_millis)
{
  timer->CR1 &= ~(1 << CEN);
  timer->EGR |= 1 << UG;

  timer->CCR[0] = delay_millis;
  timer->ARR = delay_millis;
  timer->DIER |= 1 << CC1IE;
  timer->CR1 |= 1 << CEN;

  timer_scheduled = true;
}

GPIOType *
status_setup(void)
{
  RCC_AHB2ENR |= 1 << GPIOBEN;
  GPIOType * gpio = GPIOB;

  gpio->MODER &= ~(0b11 << 2);
  gpio->MODER |= 0b01 << 2;

  gpio->MODER &= ~(0b11 << 12);
  gpio->MODER |= 0b01 << 12;

  return gpio;
}

// The button is enabled on A0
void
button_setup(void)
{
  RCC_AHB2ENR |= 1 << GPIOAEN;

  GPIOType * gpio = GPIOA;

  // A0 to input
  gpio->MODER &= ~(0b11);

  gpio->PUPDR &= ~(0b11);
  gpio->PUPDR |= 0b01;

  
  // GPIO EXTI line is 0-15
  // Unmask line 0; 
  EXTIType * exti = EXTI;
  // Ensure Line 0 is mapped to A0
  SYSCFG->EXTICR[0] &= ~(0b111);

  exti->IMR1 |= 1; // Unmask line 0
  exti->FTSR1 |= 1; // Trigger interrupt on faling bit on line 0
  NVIC->ISER[6 >> 5] |= 1 << (6 & 0x1F); // Enable EXTI Line0 interrupt
}

int
main(void)
{
  uint32_t clock_speed = clock_setup();

  button_setup();

  GPIOType * status = status_setup();
  USART_Type * usart = usart1_start(clock_speed);

  usart_write_str(usart, "USART1 Started");
  usart_write_new_line(usart);

  TIM_2_3_Type * timer = timer2_setup();

  uint8_t read_pos = 0;

  while(1)
  {
    if (button_is_pressed && !timer_scheduled)
    {
      button_is_pressed = false;
      timer_schedule(timer, 100);
    }

    if (timer_scheduled)
    {
      status->ODR |= 1 << 1;
    }
    else
    {
      status->ODR &= ~(1 << 1);
    }


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
