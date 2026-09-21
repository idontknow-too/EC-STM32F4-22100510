/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Lee Seonyu
Created          : 09-19-2026
Modified         : 09-19-2026
Language/ver     : C in VS Code

Description      : Each time Button B1 is pressed, turn on one LED at a time in a sequential loop
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

const PinName_t LED_PIN[4] = {PB_12, PB_13, PB_14, PB_15};

#define BUTTON_PIN PA_4

void setup(void) {
	RCC_HSI_init(); // Clock
	
	GPIO_init(BUTTON_PIN, INPUT);  // BUTTON_PIN(PA_4) -> enable, INPUT Mode
	GPIO_pupd(BUTTON_PIN, 01);     // BUTTON_PIN(PA_4) -> Pull Up
	
	for(int i = 0; i < 4; i++){
		GPIO_init(LED_PIN[i], OUTPUT);    // LED_PIN(PB_12, PB_13, PB_14, PB_15) -> enable, OUTPUT Mode
		GPIO_pupd(LED_PIN[i], 01);        // LED_PIN(PB_12, PB_13, PB_14, PB_15) -> Pull Up
		GPIO_otype(LED_PIN[i], 1);        // LED_PIN(PB_12, PB_13, PB_14, PB_15) -> Open drain
		GPIO_ospeed(LED_PIN[i], 01);     // LED_PIN(PB_12, PB_13, PB_14, PB_15) -> Medium speed
	}

}
	

int main(void)
{
  setup();

  int ButtonState = 1;
  int ButtonState_past = 0;
  int ButtonState_now = 0;
  int count = 0;

  int toggle = 0;

  while(1){


	if(ButtonState == 0){
		toggle = 0;
	}
	else{
		toggle = 1;
	}
	
	ButtonState_past = GPIO_read(BUTTON_PIN);

	delay_ms_HSI(30);

	ButtonState_now = GPIO_read(BUTTON_PIN);

	if((ButtonState_past == ButtonState_now) && (ButtonState_past == 0)){

		ButtonState = 0;

		if((ButtonState == 0) && (toggle == 1)){
			count = count + 1;
		
		}

		if(count >= 4){
		count = 0;
		}

	}
	else if((ButtonState_past == ButtonState_now) && (ButtonState_past != 0)){
		ButtonState = 1;
	}



	for(int i = 0; i < 4; i++){
		GPIO_write(LED_PIN[i], LOW);
		if(i == count){
			GPIO_write(LED_PIN[i], HIGH);
		}
	}

	delay_ms_HSI(40);  // 30ms delay


  }


}



