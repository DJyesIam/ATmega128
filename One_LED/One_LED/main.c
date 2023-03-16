#include <avr/io.h>

void delay(unsigned int millisec){	// ms 단위로 딜레이를 주는 함수
	volatile int i,j;
	for (i = 0; i < millisec; i++)
		for (j = 0; j < 1000; j++);
}

int main(void)
{
	DDRA = 0x01;
	
	while (1){
		PORTA |= 0x01; delay(1000);		// LED 켜고 1초 딜레이
		PORTA &= ~(0x01); delay(1000);	// LED 끄고 1초 딜레이
	}
}




