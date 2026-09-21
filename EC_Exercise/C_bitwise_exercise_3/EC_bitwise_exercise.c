#include <stdio.h>
#include <stdint.h>

void dec2bin(unsigned int n);

void main() {
    

	/*  Exercise 1  */
	printf("Exercise 1\n\n");

	unsigned char num1 = 1;    // 0000 0001
	unsigned char num2 = 3;    // 0000 0011
	unsigned char num3 = 162;  // 162: 1010 0010
	unsigned char num4 = ~num1;
	unsigned char num5 = ~num3;


	printf("%d\n", num1 & num2);
	printf("%d\n", num1 | num2);
	printf("%d\n", num1 ^ num2);
	printf("%u\n", num4);
	printf("%u\n", num5);





	/*  Exercise 2  */
	printf("\n\nExercise 2\n\n");

	num1 = 4;    // 4: 0000 0100
	num2 = 4;    // 4: 0000 0100
	num3 = 4;    // 4: 0000 0100
	num4 = 4;    // 4: 0000 0100
	num5 = 4;    // 4: 0000 0100

	num1 &= 5;     // 5(0000 0101) AND 연산 후 할당
	num2 |= 2;     // 2(0000 0010) OR 연산 후 할당
	num3 ^= 3;     // 3(0000 0011) XOR 연산 후 할당
	num4 <<= 2;    // 비트를 왼쪽으로 2번 이동한 후 할당
	num5 >>= 2;    // 비트를 오른쪽으로 2번 이동한 후 할당

	printf("%u\n", num1);
	printf("%u\n", num2);
	printf("%u\n", num3);
	printf("%u\n", num4);
	printf("%u\n", num5);




	/*  Exercise 3  */
	printf("\n\nExercise 3\n\n");


	uint8_t vals=0;

	// vals = 00000000 
	
	// Assume 8 LEDs are connected to Digital Out pins of Port A (PA)
	// LED0 at PA[0] to LED7 at PA[7]
	uint8_t PA;
	
	// Initial Values of PA
	PA = 0b00001111;			
	printf("\n Initial PA: ");
	dec2bin(PA);

	// PA = 00001111

	
	// Exercise_1-1: Turning ON LED4 at PA[4] 
	PA |=  1<<4; 	    // turn ON LED4 
	printf("\n ex1-1: ");
	dec2bin(PA);

	// PA = 00011111

	
	//Exercise_1-2:  Read the bit at PA[4], 4th from LSB
	vals =  (PA>>4)&1;	// read bit of PA[4]
	printf("\n ex1-2: %d\n", vals);

	// vals = 00000001
	
	
	// Exercise_2-1: Turn off LED4 at PA[4]
	PA &= ~(1<<4);    	// turn off LED4  
	printf("\n ex2-1: ");
	dec2bin(PA);

	// PA = 00001111
	
	//Exercise_2-2:  Read the bit at PA[4], 4th from LSB
	vals =  (PA>>4)&1;	// read bit of PA[4]
	printf("\n ex2-2: %d\n", vals);
	
	// vals = 00000000
	
	// Exercise_3-1: Turning ON LED5 and LED4,  at PA[5:4] 
	PA |=  (3<<4);	// turn ON LED5 and LED4
	printf("\n ex3-1: ");
	dec2bin(PA);

	// PA = 00111111
	
	//Exercise_3-2:  Read bits LED5 and LED4,  at PA[5:4] 
	vals =  (PA>>4) & 3;	// read bits at PA[5:4] 
	printf("\n ex3-2: %d\n", vals);

	// vals = 00000011

	
	//Exercise_4-1:  SET  PA[0] and PA[7] 
	PA |= ((1<<7) | (1<<0)); 
	printf("\n ex4-1: ");
	dec2bin(PA);

	// PA = 10111111
	
	//Exercise_4-2:  CLEAR  PA[0] and PA[7] 
	PA &=  ~((1<<7) | 1);
	printf("\n ex4-2: ");
	dec2bin(PA);
	
	// PA = 00111110
	
	//Exercise_5-1:  Toggle LED7 PA[7]
	printf("\n");    
	PA ^=  (1<<7);
	printf("\n ex5-1: ");
	dec2bin(PA);

	// PA = 10111110
	
	printf("\n");    
	
}

void dec2bin(unsigned int n) {
	unsigned int a = 0x80;
	for (int i = 0; i < 8; i++) {
		if ((a & n) == a)
			printf("1");
		else
			printf("0");
		a = a >> 1;
	}
}
