#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>

// ###################
// #### RCC ##########
// ###################

#define RCC_ADDRESS 0x40021000

#define USART1EN 14
#define RCC_APB2ENR_ADDRESS (RCC_ADDRESS + 0x60)
#define RCC_APB1ENR1_ADDRESS (RCC_ADDRESS + 0x58)
#define RCC_AHB2ENR_ADDRESS (RCC_ADDRESS + 0x4c)

#define RCC_APB2ENR *((uint32_t *) RCC_APB2ENR_ADDRESS)

#define TIM2EN 0
#define RCC_APB1ENR1 *((uint32_t *) RCC_APB1ENR1_ADDRESS)

#define GPIOAEN 0
#define RCC_AHB2ENR *((uint32_t *) RCC_AHB2ENR_ADDRESS)


// ###################
// #### TIMERS #######
// ###################

#define CEN 0
#define CNT_EN

#define TIM2_ADDRESS 0x40000000
#define TIM2 ((TIM2_Type *) TIM2_ADDRESS)


typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SMCR;
  volatile uint32_t DIER;
  volatile uint32_t SR;
  volatile uint32_t EGR;
  volatile uint32_t CCMR[2];
  volatile uint32_t CCER;
  volatile uint32_t CNT;
  volatile uint32_t PSC;
  volatile uint32_t ARR;
  volatile uint32_t CCR[4];
  volatile uint32_t DCR;
  volatile uint32_t DMAR;
  volatile uint32_t OR[2];
} TIM2_Type;

// ###################
// #### GPIO #########
// ###################

#define GPIOA_ADDRESS 0x48000000

#define GPIOA ((GPIOType *) GPIOA_ADDRESS)

typedef struct {
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFRL;
  volatile uint32_t AFRH;
} GPIOType;

#endif
