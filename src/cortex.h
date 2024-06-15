#ifndef CORTEX_H
#define CORTEX_H

#define _NVIC_BASE 0xE000E100
#define NVIC ((NVICType *) _NVIC_BASE)

typedef struct
{
  volatile uint32_t ISER[8]; // One way of representing 256 bits
  volatile uint32_t ICER[8];
  volatile uint32_t ISPR[8];
  volatile uint32_t ICPR[8];
  volatile uint32_t IABR[8];
  volatile uint32_t IPR[8];
} NVICType;

#endif
