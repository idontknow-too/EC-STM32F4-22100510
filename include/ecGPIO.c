/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Lee Seonyu
Created          : 09-17-2026
Modified         : 09-17-2026
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_7-segment
/----------------------------------------------------------------*/

// FND - JKIT board
static PinName_t _fndPins[8];
static PinName_t _fndSel[4];

//Each led that has to light up gets a 1, every other led gets a 0
//its in order  { (a,b,c,d,e,f,g,dp) }
int numberFND[12][8]={
                    {1,1,1,1,1,1,0,0},          //zero
                    {0,1,1,0,0,0,0,0},          //one
                    {1,0,1,1,1,0,1,0},          //two
                    {1,1,1,1,0,0,1,0},          //three
                    {0,1,1,0,0,1,1,0},          //four
                    {1,0,1,1,0,1,1,0},          //five
                    {1,0,1,1,1,1,1,0},          //six
                    {1,1,1,0,0,1,0,0},          //seven
                    {1,1,1,1,1,1,1,0},          //eight
                    {1,1,1,1,0,1,1,0},          //nine
                    {0,0,0,0,0,0,0,1}          //dot
                   	{0,0,0,0,0,0,0,0}		      //blank
                  };
                  

// Initialize DOUT pins for 7 segment leds
void FND_init(PinName_t *pinFND){
	for (int i=0; i<8; i++){

		_fndPins[i] = pinFND[i];
		GPIO_init(_fndPins[i], OUTPUT);
        GPIO_ospeed(_fndPins[i], 01);
        
	}
}

// Initialize select pins (FND0~FND3) as DOUT
void FND_select_init(PinName_t *selectFND){	
    // [YOUR CODE GOES HERE]
    // [YOUR CODE GOES HERE]

    for (int i=0; i<4; i++){

        _fndSel[i] = selectFND[i]; // selectFND[0] = PB_0, selectFND[1] = PB_1, ...
		GPIO_init(_fndsel[i], OUTPUT);
        GPIO_otyper(_fndsel[i], 0);

    }
}

// Display a number 0 - 9 only
void FND_display(uint8_t num){
    // [YOUR CODE GOES HERE]    
    // [YOUR CODE GOES HERE]
    
    if(num > 9){

        printf("It can't display. Too high.");

    }  
    else if(num < 0){

        printf("It can't display. Too low.");

    }
    else{
        
        for(int i = 0; i < 8; i++){

            GPIO_write(_fndPins[i], numberFND[num][i]);

        }
    }

}

// Select display: FND0 to FND3 하나만 키고 나머지는 끄기
void FND_select(uint8_t digit){
    // [YOUR CODE GOES HERE]    
    // [YOUR CODE GOES HERE]
    
    for(int i = 0; i < 4; i++){

        if(i == digit){
            GPIO_write(_fndSel[i], 1);

        }
        else{
            GPIO_write(_fndSel[i], 0);

        }
    }

}

////////////////////////////////////////////////////////////////////




