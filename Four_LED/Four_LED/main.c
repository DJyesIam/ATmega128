// 네 개의 LED를 순서대로 켰다가 꺼지게 한다.


void delay(unsigned int millisec){
	volatile int i,j;
	
	for (i = 0; i < millisec; i++)
		for (j = 0; j < 1000; j++);
}

#include <avr/io.h>

int main(void)
{
	DDRA = 0x0F;	// A0, A1, A2, A3 출력 모드
	PORTA = 0x00;	// 출력 상태 0으로 초기화
    while (1) 
    {
		PORTA &= ~(0x01);
		PORTA |= 0x01 << 1;
		delay(1000);
		PORTA &= ~(0x01 << 1);
		PORTA |= 0x01 << 2;
		delay(1000);
		PORTA &= ~(0x01 << 2);
		PORTA |= 0x01 << 3;
		delay(1000);
		PORTA &= ~(0x01 << 3);
		PORTA |= 0x01 << 2;
		delay(1000);
		PORTA &= ~(0x01 << 2);
		PORTA |= 0x01 << 1;
		delay(1000);
		PORTA &= ~(0x01 << 1);
		PORTA |= 0x01;
		delay(1000);
    }
}

