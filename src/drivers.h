#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>

// ###################
// #### RCC ##########
// ###################

#define RCC_ADDRESS 0x40021000

#define RCC_CR_ADDRESS RCC_ADDRESS
#define RCC_CFGR_ADDRESS (RCC_ADDRESS + 0x08)
#define RCC_APB2ENR_ADDRESS (RCC_ADDRESS + 0x60)
#define RCC_APB1ENR1_ADDRESS (RCC_ADDRESS + 0x58)
#define RCC_AHB2ENR_ADDRESS (RCC_ADDRESS + 0x4c)
#define RCC_BDCR_ADDRESS (RCC_ADDRESS + 0x90)

#define USART1EN 14
#define RCC_APB2ENR *((uint32_t *) RCC_APB2ENR_ADDRESS)

#define TIM2EN 0
#define PWREN 28
#define RCC_APB1ENR1 *((uint32_t *) RCC_APB1ENR1_ADDRESS)

#define GPIOAEN 0
#define GPIOBEN 1
#define RCC_AHB2ENR *((uint32_t *) RCC_AHB2ENR_ADDRESS)

#define RTCEN 15
#define RCC_BDCR *((uint32_t *) RCC_BDCR_ADDRESS)

#define MSION 1
#define MSIRANGE 4
#define MSIPLLEN 2
#define MSIRDY 1
#define RCC_CR *((uint32_t *) RCC_CR_ADDRESS)

#define SW 0
#define RCC_CFGR *((uint32_t *) RCC_CFGR_ADDRESS)

// ###################
// #### PWR ##########
// ###################

#define PWR_ADDRESS 0x40007000
#define PWR_CR1_ADDRESS PWR_ADDRESS

#define VOS 9
#define DBP 8

#define PWR_CR1 *((uint32_t *) PWR_CR1_ADDRESS)


// ###################
// #### TIMERS #######
// ###################

#define TIM2_ADDRESS 0x40000000

#define CEN 0
#define OPM 3
#define CC1IE 1
#define UIE 0
#define CC1IF 1
#define UIF 0
#define UG 0
#define TIM2 ((TIM_2_3_Type *) TIM2_ADDRESS)

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
} TIM_2_3_Type;

// ###################
// #### GPIO #########
// ###################

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

#define GPIOA_ADDRESS 0x48000000
#define GPIOB_ADDRESS 0x48000400

#define GPIOA ((GPIOType *) GPIOA_ADDRESS)
#define GPIOB ((GPIOType *) GPIOB_ADDRESS)

// ###################
// #### GPIO #########
// ###################

typedef struct {
  volatile uint32_t IMR1;
  volatile uint32_t EMR1;
  volatile uint32_t RTSR1;
  volatile uint32_t FTSR1;
  volatile uint32_t SWIER1;
  volatile uint32_t PR1;
  volatile uint32_t IMR2;
  volatile uint32_t EMR2;
  volatile uint32_t RTSR2;
  volatile uint32_t FTSR2;
  volatile uint32_t SWIER2;
  volatile uint32_t PR2;
} EXTIType;

#define EXTI_ADDRESS 0x40010400
#define EXTI ((EXTIType * ) EXTI_ADDRESS)

// ###################
// #### SYSCFG #######
// ###################

typedef struct {
  volatile uint32_t MEMRMP;
  volatile uint32_t CFGR1;
  volatile uint32_t EXTICR[4];
  volatile uint32_t SCSR;
  volatile uint32_t CFGR2;
  volatile uint32_t SWPR;
  volatile uint32_t SKR;
} SYSCFGType;

#define SYSCFG_ADDRESS 0x40010000
#define SYSCFG ((SYSCFGType *) SYSCFG_ADDRESS)

#endif
