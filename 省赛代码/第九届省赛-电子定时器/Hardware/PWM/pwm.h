#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

extern __IO uint16_t CCR1_Val;
extern float PA6_Duty;


void pwm_init(uint8_t PA6);

#endif /*__PWM_H*/
