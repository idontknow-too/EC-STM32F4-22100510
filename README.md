"# Embedded Controller - STM32f411 HAL API" 

Documentation for HAL functions

Written by: Lee Seonyu

Course: Embedded Controller

Program: C/C++

IDE/Compiler: Keil uVision 5

OS: Win11 Home

MCU: STM32F411RE (Nucleo-64)


[TOC]

GPIO Digital In/Out
Header File
GPIO_init()
GPIO_mode()
GPIO_write()
GPIO_read()
GPIO_ospeed()
GPIO_otype()
GPIO_pupd()

---

"# GPIO Digital InOut"

--

# Header File


#include "ecGPIO.h"

#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

#define HIGH 1
#define LOW  0


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, uint32_t mode);     
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
void GPIO_otype(PinName_t pinName, uint32_t type);
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
void GPIO_write(PinName_t pinName, uint32_t Output);
uint32_t  GPIO_read(PinName_t pinName);

// ARDUINO Syntax  (Optional)
void pinMode(PinName_t pin, uint32_t mode);
void digitalWrite(PinName_t pin, uint32_t value);
uint32_t  digitalRead(PinName_t pin);

 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H











