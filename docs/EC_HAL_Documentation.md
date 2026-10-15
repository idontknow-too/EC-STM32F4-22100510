# Embedded Controller - STM32F411 HAL API

Written by: Lee Seonyu (22100510)  
Course: Embedded Controller, Handong Global University  
Language: C  
IDE / Build tools: Visual Studio Code, PlatformIO, ARM GCC  
OS: Windows  
MCU: STM32F411RE (NUCLEO-F411RE)  
Updated: 2026-10-15

Function reference for the current libraries in `include/`, covering the GPIO, seven-segment display, EXTI, SysTick and stepper motor LABs. Each function is described by its purpose, declaration, parameters and example code. The organization follows the [course documentation example](https://github.com/ykkimhgu/EC-student/blob/main/docs/EC_HAL_Documentation.md); signatures and behavior follow this repository's source files.

## Contents

- [GPIO Digital InOut](#gpio-digital-inout)
- [System Clock and RCC](#system-clock-and-rcc)
- [Pin Mapping](#pin-mapping)
- [Seven-Segment Display](#seven-segment-display)
- [External Interrupts](#external-interrupts)
- [SysTick and Delay](#systick-and-delay)
- [Stepper Motor](#stepper-motor)
- [LAB_EXTI Board Initialization](#lab_exti-board-initialization)
- [Declarations without implementations](#declarations-without-implementations)

## Using the Examples

Include the header shown for each module and build its source file together with its dependencies. The snippets show statements to use during setup or in the main loop; they are not separate complete programs. All functions returning `void` have no return value. Non-void return values are described individually.

Use valid pins and the documented parameter ranges. Unless stated otherwise, the functions do not check for invalid values. Use module-specific pin assignments: the display and stepper examples share some pins and are intended for separate LAB programs.

## GPIO Digital InOut

Configure and read or write individual GPIO pins using `PinName_t` values such as `PA_5` and `PB_12`.

### Header File

```c
#include "ecGPIO2.h"
```

[Header](../include/ecGPIO2.h) · [Source](../include/ecGPIO2.c)

| Setting | Values |
| --- | --- |
| Mode | `INPUT` (0), `OUTPUT` (1), `AF` (2), `ANALOG` (3) |
| Output level | `LOW` (0), `HIGH` (1) |
| Output speed | `SPD_L` (0), `SPD_M` (1), `SPD_F` (2), `SPD_H` (3) |
| Output type | 0: push-pull, 1: open-drain |
| Pull resistor | `NO_PUPD` (0), `PU` (1), `PD` (2) |

Call `GPIO_init()` before configuring or using a pin. GPIO output speed controls the electrical output setting; it does not set a blink rate or motor speed.

### GPIO_init()

Enable the selected port's clock and set the pin mode. Output type, speed and pull resistors are configured separately.

```c
void GPIO_init(PinName_t pinName, uint32_t mode);
```

**Parameters**

- `pinName`: GPIO pin, for example `PB_12`.
- `mode`: `INPUT`, `OUTPUT`, `AF` or `ANALOG`.

**Example code**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_init(PC_13, INPUT);
```

### GPIO_mode()

Set the pin's two mode bits in `MODER`.

```c
void GPIO_mode(PinName_t pinName, uint32_t mode);
```

**Parameters**

- `pinName`: GPIO pin with its port clock already enabled.
- `mode`: `INPUT`, `OUTPUT`, `AF` or `ANALOG`.

**Example code**

```c
GPIO_init(PA_5, OUTPUT);
GPIO_mode(PA_5, INPUT);
```

Selecting `AF` changes the mode only; this function does not configure the alternate-function number in `AFR`.

### GPIO_write()

Write a digital level to the pin's output data register.

```c
void GPIO_write(PinName_t pinName, uint32_t Output);
```

**Parameters**

- `pinName`: GPIO output pin.
- `Output`: `LOW` (0) or `HIGH` (1).

**Example code**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_write(PB_12, HIGH);
```

The implementation updates `ODR` using read-modify-write operations. Concurrent writes to the same port need coordination.

### GPIO_read()

Read the pin level from `IDR`.

```c
uint32_t GPIO_read(PinName_t pinName);
```

**Parameters**

- `pinName`: GPIO pin to read.

**Return value**

`LOW` (0) or `HIGH` (1).

**Example code**

```c
GPIO_init(PC_13, INPUT);
GPIO_pupd(PC_13, PU);
uint32_t button = GPIO_read(PC_13);
(void)button;
```

### GPIO_ospeed()

Set the pin's output speed in `OSPEEDR`.

```c
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
```

**Parameters**

- `pinName`: GPIO pin.
- `speed`: `SPD_L`, `SPD_M`, `SPD_F` or `SPD_H`.

**Example code**

```c
GPIO_init(PB_10, OUTPUT);
GPIO_ospeed(PB_10, SPD_F);
```

### GPIO_otype()

Set push-pull or open-drain output operation.

```c
void GPIO_otype(PinName_t pinName, uint32_t type);
```

**Parameters**

- `pinName`: GPIO pin.
- `type`: 0 for push-pull; 1 for open-drain.

**Example code**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_otype(PB_12, 0); // Push-pull.
```

### GPIO_pupd()

Set the internal pull-up or pull-down resistor configuration.

```c
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
```

**Parameters**

- `pinName`: GPIO pin.
- `pupd`: `NO_PUPD`, `PU` or `PD`.

**Example code**

```c
GPIO_init(PA_4, INPUT);
GPIO_pupd(PA_4, PU);
```

### led_toggle()

Toggle JKIT LED0 on `PB_12` by XORing its output bit.

```c
void led_toggle(PinName_t pinName);
```

**Parameters**

- `pinName`: Accepted by the interface, but the current implementation always toggles `PB_12`. Pass `PB_12` for this version.

**Example code**

```c
GPIO_init(PB_12, OUTPUT);
led_toggle(PB_12);
```

This version is specific to `PB_12`; passing another pin does not change the output pin.

## System Clock and RCC

Select the system clock and enable GPIO peripheral clocks.

### Header File

```c
#include "ecRCC2.h"
```

[Header](../include/ecRCC2.h) · [Source](../include/ecRCC2.c)

`extern volatile int EC_SYSCLK;` stores the clock frequency in hertz. Its initial value is 16000000.

Call one clock initializer during startup, before `SysTick_init()`. These examples assume the reset clock configuration. `RCC_HSI_init()` does not reset the bus prescalers, and `RCC_PLL_init()` is intended for startup rather than repeated switching while PLL is already the clock source.

`GPIO_init()` already calls the appropriate GPIO clock-enable function. The individual enable functions below do not configure any pin.

### RCC_HSI_init()

Enable the 16 MHz HSI oscillator, wait for it to become ready and select it as the system clock. Set `EC_SYSCLK` to 16000000.

```c
void RCC_HSI_init(void);
```

**Parameters**

None.

**Example code**

```c
RCC_HSI_init();
```

### RCC_PLL_init()

Use HSI with PLLM = 8, PLLN = 84 and PLLP = 2 to select an 84 MHz system clock. Set AHB to 84 MHz, APB1 to 42 MHz, APB2 to 84 MHz and `EC_SYSCLK` to 84000000.

```c
void RCC_PLL_init(void);
```

**Parameters**

None.

**Example code**

```c
RCC_PLL_init();
```

### delay_ms_HSI()

Wait using a software decrement loop whose count is calculated from `EC_SYSCLK`.

```c
void delay_ms_HSI(uint32_t ms);
```

**Parameters**

- `ms`: Requested delay in milliseconds.

**Example code**

```c
RCC_HSI_init();
delay_ms_HSI(1000);
```

The duration is approximate and depends on generated instructions and compiler optimization. Use the SysTick-based `delay_ms()` for the LAB's millisecond time base.

### RCC_GPIOA_enable()

Enable the GPIOA peripheral clock in `RCC->AHB1ENR`.

```c
void RCC_GPIOA_enable(void);
```

**Parameters**

None.

**Example code**

```c
RCC_GPIOA_enable();
```

### RCC_GPIOB_enable()

Enable the GPIOB peripheral clock in `RCC->AHB1ENR`.

```c
void RCC_GPIOB_enable(void);
```

**Parameters**

None.

**Example code**

```c
RCC_GPIOB_enable();
```

### RCC_GPIOC_enable()

Enable the GPIOC peripheral clock in `RCC->AHB1ENR`.

```c
void RCC_GPIOC_enable(void);
```

**Parameters**

None.

**Example code**

```c
RCC_GPIOC_enable();
```

### RCC_GPIOD_enable()

Enable the GPIOD peripheral clock in `RCC->AHB1ENR`.

```c
void RCC_GPIOD_enable(void);
```

**Parameters**

None.

**Example code**

```c
RCC_GPIOD_enable();
```

### RCC_GPIOE_enable()

Enable the GPIOE peripheral clock in `RCC->AHB1ENR`.

```c
void RCC_GPIOE_enable(void);
```

**Parameters**

None.

**Example code**

```c
RCC_GPIOE_enable();
```

### RCC_GPIOH_enable()

Enable the GPIOH peripheral clock in `RCC->AHB1ENR`.

```c
void RCC_GPIOH_enable(void);
```

**Parameters**

None.

**Example code**

```c
RCC_GPIOH_enable();
```

## Pin Mapping

`PinName_t` identifies a port and pin in one value. For example, `PB_10` is `0x1A`: port B is in the upper bits and pin 10 is in the lower four bits.

### Header File

```c
#include "ecPinNames.h"
```

[Header](../include/ecPinNames.h) · [Source](../include/ecPinNames.c)

The header also provides Arduino connector aliases such as `D13` (`PA_5`). Use connected pins defined in the header; `NC` is not a usable GPIO pin.

### ecPinmap()

Decode a pin name into a GPIO register-block pointer and a pin number. The implementation recognizes port codes A, B, C, D, E and H.

```c
void ecPinmap(PinName_t pinName, GPIO_TypeDef **GPIOx, unsigned int *pin);
```

**Parameters**

- `pinName`: Encoded pin name.
- `GPIOx`: Address of the variable that receives the GPIO port pointer.
- `pin`: Address of the variable that receives the pin number, 0 to 15.

**Example code**

```c
GPIO_TypeDef *port;
unsigned int pin;
ecPinmap(PB_10, &port, &pin);
// port == GPIOB, pin == 10.
```

Output pointers must be valid. Unrecognized port codes fall back to GPIOA; the function does not report invalid pins.

## Seven-Segment Display

Control the JKIT display's eight segment outputs and four digit-selection outputs.

### Header File

```c
#include "ecGPIO.h"
```

[Header](../include/ecGPIO.h) · [Source](../include/ecGPIO.c)

Segment order is `a, b, c, d, e, f, g, dp`. Digit-selection order is FND0 to FND3. Both segment and digit selection outputs use HIGH for activation in this implementation.

`extern int numberFND[12][8];` holds patterns for digits 0 to 9, a decimal point (index 10) and blank (index 11). `FND_display()` accepts only 0 to 9 in the current implementation. Multiplexing several digits requires repeated selection and display updates in the application.

### FND_init()

Store the eight segment pin names and configure them as push-pull outputs with no pull resistors and medium output speed.

```c
void FND_init(PinName_t *pinFND);
```

**Parameters**

- `pinFND`: Pointer to eight pin names in `a, b, c, d, e, f, g, dp` order.

**Example code**

```c
PinName_t segments[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
FND_init(segments);
```

### FND_select_init()

Store the four digit-selection pin names and configure them as push-pull outputs.

```c
void FND_select_init(PinName_t *selectFND);
```

**Parameters**

- `selectFND`: Pointer to four pin names in FND0 to FND3 order.

**Example code**

```c
PinName_t digits[4] = {PA_10, PA_11, PC_4, PC_3};
FND_select_init(digits);
```

### FND_display()

Write the selected digit's pattern to the eight segment pins. The digit selection is controlled separately by `FND_select()`.

```c
void FND_display(uint8_t num);
```

**Parameters**

- `num`: Digit from 0 to 9.

**Example code**

```c
PinName_t segments[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t digits[4] = {PA_10, PA_11, PC_4, PC_3};
FND_init(segments);
FND_select_init(digits);
FND_select(0);
FND_display(5);
```

For values above 9, the function prints an error message and leaves the segment outputs unchanged.

### FND_select()

Set the selected digit's selection output HIGH and the other three outputs LOW.

```c
void FND_select(uint8_t digit);
```

**Parameters**

- `digit`: 0, 1, 2 or 3 for FND0 to FND3.

**Example code**

```c
PinName_t segments[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t digits[4] = {PA_10, PA_11, PC_4, PC_3};
FND_init(segments);
FND_select_init(digits);
FND_display(3);
FND_select(1);
```

A value greater than 3 sets all four selection outputs LOW.

## External Interrupts

Map GPIO inputs to EXTI lines, select trigger edges and control interrupt masking.

### Header File

```c
#include "ecEXTI2.h"
#include "ecGPIO2.h"
```

[Header](../include/ecEXTI2.h) · [Source](../include/ecEXTI2.c)

| Trigger | Value | Edge |
| --- | --- | --- |
| `FALL` | 0 | HIGH to LOW |
| `RISE` | 1 | LOW to HIGH |
| `BOTH` | 2 | Both edges |

Configure the GPIO input and pull resistor separately. Pins 0 to 4 use individual IRQ handlers; pins 5 to 9 share `EXTI9_5_IRQHandler()`, and pins 10 to 15 share `EXTI15_10_IRQHandler()`. Only one port can be mapped to a given EXTI line number at a time.

The current port-selection code handles GPIOA to GPIOE; GPIOH is not correctly routed by this version. Trigger initialization sets the requested edge bits without clearing previously enabled edges, so it assumes a fresh trigger configuration.

### EXTI_init()

Enable the SYSCFG clock, map the input to its EXTI line, enable the requested trigger, unmask the line and enable its NVIC interrupt.

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority);
```

**Parameters**

- `pinName`: GPIO input pin.
- `trig_type`: `FALL`, `RISE` or `BOTH`.
- `priority`: NVIC priority from 0 to 15; a smaller number has higher priority.

**Example code**

```c
GPIO_init(PC_13, INPUT);
GPIO_pupd(PC_13, PU);
EXTI_init(PC_13, FALL, 10);
```

The application must supply the matching IRQ handler and clear the pending flag there. `EXTI_init()` does not debounce a button.

### EXTI_enable()

Unmask the selected EXTI line in `IMR` after it has been configured.

```c
void EXTI_enable(PinName_t pinName);
```

**Parameters**

- `pinName`: Pin identifying the EXTI line to enable.

**Example code**

```c
EXTI_enable(PC_13);
```

### EXTI_disable()

Mask the selected EXTI line in `IMR`. GPIO input reads remain available.

```c
void EXTI_disable(PinName_t pinName);
```

**Parameters**

- `pinName`: Pin identifying the EXTI line to disable.

**Example code**

```c
EXTI_disable(PC_13);
```

This does not clear a pending flag or disable other lines sharing the same NVIC interrupt.

### is_pending_EXTI()

Read the selected line's pending bit in `EXTI->PR`.

```c
uint32_t is_pending_EXTI(PinName_t pinName);
```

**Parameters**

- `pinName`: Pin identifying the EXTI line to check.

**Return value**

1 if the line has a pending event; otherwise 0.

**Example code**

```c
if (is_pending_EXTI(PC_13)) {
    clear_pending_EXTI(PC_13);
}
```

### clear_pending_EXTI()

Clear the selected EXTI pending bit by writing 1 to that bit in `PR`.

```c
void clear_pending_EXTI(PinName_t pinName);
```

**Parameters**

- `pinName`: Pin identifying the pending flag to clear.

**Example code**

```c
clear_pending_EXTI(PC_13);
```

## SysTick and Delay

Maintain a millisecond software counter and provide blocking delays using the Cortex-M SysTick timer.

### Header File

```c
#include "ecSysTick2.h"
```

[Header](../include/ecSysTick2.h) · [Source](../include/ecSysTick2.c)

`extern volatile uint32_t msTicks;` counts serviced millisecond ticks. Call `SysTick_init()` after choosing the clock source. Keep SysTick interrupts able to run while waiting in `delay_ms()`; it cannot finish if the tick is prevented from advancing.

The library already defines `SysTick_Handler()`. Do not add a second definition to an application that links `ecSysTick2.c`.

### SysTick_init()

Configure the processor-clock SysTick counter with `LOAD = EC_SYSCLK / 1000 - 1`, enable its interrupt and start counting. The SysTick priority is 15.

```c
void SysTick_init(void);
```

**Parameters**

None.

**Example code**

```c
RCC_PLL_init();
SysTick_init();
```

Reinitialization reloads the hardware counter but does not set `msTicks` to zero.

### SysTick_Handler()

Handle the SysTick exception by calling `SysTick_counter()`. With the configured time base, the handler is requested every 1 ms.

```c
void SysTick_Handler(void);
```

**Parameters**

None.

**Example code**

```c
RCC_PLL_init();
SysTick_init();
// SysTick_Handler() is then invoked by the interrupt mechanism.
```

This is an interrupt entry point supplied by the library, not a function normally called from `main()`.

### SysTick_counter()

Increment `msTicks` by one. The library's SysTick handler calls this once per serviced tick.

```c
void SysTick_counter(void);
```

**Parameters**

None.

**Example code**

```c
// Inside the library's SysTick_Handler():
SysTick_counter();
```

Do not also call it from the main loop; extra calls advance the software clock without waiting a millisecond.

### delay_ms()

Block until the unsigned difference between `msTicks` and its starting value reaches `msec`.

```c
void delay_ms(uint32_t msec);
```

**Parameters**

- `msec`: Requested wait in milliseconds; 0 returns immediately.

**Example code**

```c
RCC_PLL_init();
SysTick_init();
delay_ms(1000);
```

Resolution is 1 ms. Unsigned subtraction supports counter wrap-around. Avoid calling this inside an interrupt that prevents the lower-priority SysTick handler from running.

### SysTick_reset()

Write zero to the hardware SysTick current-value register (`VAL`). The timer reloads on a subsequent clock.

```c
void SysTick_reset(void);
```

**Parameters**

None.

**Example code**

```c
RCC_PLL_init();
SysTick_init();
SysTick_reset();
```

This does not reset the software counter `msTicks`.

### SysTick_val()

Read the current hardware down-counter value from `SysTick->VAL`.

```c
uint32_t SysTick_val(void);
```

**Parameters**

None.

**Return value**

Current SysTick counter value in processor-clock ticks, not elapsed milliseconds.

**Example code**

```c
RCC_PLL_init();
SysTick_init();
uint32_t count = SysTick_val();
(void)count;
```

## Stepper Motor

Drive a 28BYJ-48 through four ULN2003 inputs using a Moore FSM. The pin and output order is `A, B, A', B'`.

### Header File

```c
#include "ecStepper.h"
```

[Header](../include/ecStepper.h) · [Source](../include/ecStepper.c)

| Setting | Value |
| --- | --- |
| `HALF` | 0; eight FSM states |
| `FULL` | 1; four FSM states |
| `DIR_CW` | 0; next state in the forward sequence |
| `DIR_CCW` | 1; next state in the reverse sequence |
| `STEP_PER_REV` for Full | `64*32` = 2048 |
| `STEP_PER_REV` for Half | `64*32*2` = 4096 |

The current header defines `STEP_PER_REV` as 4096 for Half stepping. Change this macro to match the mode before building a Full-stepping program. `stepper_init()` selects the FSM; it does not change this macro.

`StateNum` defines `S0` to `S7`. Each `StateStepper_t` contains `next[2]` for the direction-dependent next states and `out[4]` for the four GPIO levels. CW advances through increasing state numbers with wrap-around; CCW follows the reverse order. Physical direction depends on motor wiring.

Initialize the clock and SysTick before movement. Choose the stepping mode once during startup: `stepper_init()` does not reset the saved state, so this implementation should not switch from Half to Full during an active session.

```c
RCC_PLL_init();
SysTick_init();
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins); // Matches the current STEP_PER_REV = 4096.
stepper_speed(1.0f);
stepper_step(STEP_PER_REV, DIR_CW);
stepper_stop();
```

### stepper_init()

Select the Full or Half FSM, store the four motor pin names and configure them as outputs with `SPD_F` output speed.

```c
void stepper_init(uint32_t mode, PinName_t *pinStepper);
```

**Parameters**

- `mode`: `FULL` or `HALF`.
- `pinStepper`: Pointer to four pin names in `A, B, A', B'` order.

**Example code**

```c
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins);
```

GPIO output speed is separate from motor rpm. Set rpm with `stepper_speed()` before starting a movement.

### stepper_speed()

Calculate the delay per step as `60000 / (STEP_PER_REV * speedRPM)` in milliseconds and store it as an integer. The call sets the speed; it does not start movement or wait.

```c
void stepper_speed(float speedRPM);
```

**Parameters**

- `speedRPM`: Positive requested speed in revolutions per minute, including decimal values such as `0.1f` or `14.6f`.

**Example code**

```c
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins);
stepper_speed(1.0f);
```

The result is truncated to whole milliseconds. Use a positive finite rpm that gives a representable delay of at least 1 ms; zero and out-of-range speeds are not checked. The requested rpm can differ from the actual speed because of delay rounding and motor limitations.

### stepper_step()

For each requested step, select the next FSM state, write its four outputs and wait for the stored step delay. The function blocks until the movement finishes or its active step command is cancelled.

```c
void stepper_step(uint32_t steps, uint32_t direction);
```

**Parameters**

- `steps`: Number of sequence transitions to execute. `STEP_PER_REV` requests one nominal revolution when the macro matches the mode.
- `direction`: `DIR_CW` (0) or `DIR_CCW` (1).

**Example code**

```c
RCC_PLL_init();
SysTick_init();
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins);
stepper_speed(1.0f);
stepper_step(STEP_PER_REV, DIR_CW);
```

Returns without movement if initialization has not selected an FSM. Direction must be 0 or 1. The state is advanced before its output is written; the initial saved state is `S0`. Outputs remain energized after normal completion.

### stepper_out()

Write the selected FSM state's `out[4]` values to the four motor GPIO pins in order. It does not change the saved state used by `stepper_step()`.

```c
void stepper_out(uint32_t state);
```

**Parameters**

- `state`: `S0` to `S3` for Full stepping, or `S0` to `S7` for Half stepping.

**Example code**

```c
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins);
stepper_out(S0); // Half output: HIGH, LOW, LOW, LOW.
```

Call `stepper_init()` first. This function does not validate its state index and is normally called by `stepper_step()`.

### stepper_stop()

Set the active command's step limit to zero and write LOW to all four motor pins.

```c
void stepper_stop(void);
```

**Parameters**

None.

**Example code**

```c
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins);
stepper_stop();
```

A delay already in progress finishes before the step loop exits. The stop is not latched: a later `stepper_step()` call starts a new command. If an interrupt calls stop during `stepper_out()`, remaining GPIO writes can resume after the interrupt; those writes are not synchronized with stop.

## LAB_EXTI Board Initialization

`MCU_init()` is a JKIT setup helper for LAB_EXTI. It configures that LAB's LEDs, switches and display.

### Header File

```c
#include "ecSTM32F4v2.h"
```

[Header](../include/ecSTM32F4v2.h)

| Peripheral | Pins |
| --- | --- |
| LED0, LED1 | `PB_12`, `PB_13` |
| SW2, SW1 | `PA_4`, `PD_2` |
| Segments a to dp | `PB_0` to `PB_7` |
| FND0 to FND3 selection | `PA_10`, `PA_11`, `PC_4`, `PC_3` |

This header contains the `MCU_init()` definition and the global `pinFND` and `selectFND` arrays. Include it from only one C source file in a program to avoid multiple definitions. It is not a general initializer for every LAB; the segment pins overlap the stepper motor pins.

### MCU_init()

Initialize the 84 MHz PLL clock and 1 ms SysTick, configure LED0 and LED1 as outputs, configure SW2 and SW1 as pull-up inputs with falling-edge interrupts at priority 10, and initialize the display pins.

```c
void MCU_init(void);
```

**Parameters**

None.

**Example code**

```c
MCU_init();
```

The application supplies `EXTI4_IRQHandler()` for SW2 and `EXTI2_IRQHandler()` for SW1. The helper does not choose a displayed digit or start a display-refresh loop.

## Declarations without Implementations

The following functions are declared in the current headers but have no active definitions in the library source files. They are excluded from the implemented API above; calling them requires adding an implementation first.

| Header | Declaration |
| --- | --- |
| `ecGPIO2.h` | `void pinMode(PinName_t pin, uint32_t mode);` |
| `ecGPIO2.h` | `void digitalWrite(PinName_t pin, uint32_t value);` |
| `ecGPIO2.h` | `uint32_t digitalRead(PinName_t pin);` |
| `ecSysTick2.h` | `void delay_us(uint32_t usec);` |

The current stepper motor program uses the implemented millisecond `delay_ms()` function.
