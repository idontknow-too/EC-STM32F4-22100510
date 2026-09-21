/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Lee Seonyu
Created          : 05-03-2021
Modified         : 09-17-2026
Language/ver     : C++ in VS Code

Description      : Turn on the LED by covering the photodetector sensor and turn off the LED when uncovering the photodetector
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN PB_12
#define BUTTON_PIN PA_0


void setup(void) {
	RCC_HSI_init(); // Clock
	
	GPIO_init(BUTTON_PIN, INPUT);  // BUTTON_PIN(PA_0) -> enable, INPUT Mode
	GPIO_pupd(BUTTON_PIN, 01);     // BUTTON_PIN(PA_0) -> Pull Up
	
	GPIO_init(LED_PIN, OUTPUT);    // LED_PIN(PB_12) -> enable, OUTPUT Mode
	GPIO_pupd(LED_PIN, 01);        // LED_PIN(PB_12) -> Pull Up
	GPIO_otype(LED_PIN, 1);        // LED_PIN(PB_12) -> Open drain
	GPIO_ospeed(LED_PIN, 01);     // LED_PIN(PB_12) -> Medium speed


}
	
int main(void) { 
 	setup();
	int sensorstate = 0;


	while(1){

		sensorstate = GPIO_read(BUTTON_PIN);
		
		if(sensorstate == 0){

			GPIO_write(LED_PIN, HIGH);

		}

		else{

			GPIO_write(LED_PIN, LOW);

		}


		delay_ms_HSI(10);  // clock 10ms

	}


}