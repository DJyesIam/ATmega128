// 스위치를 누를 때마다 1씩 증가되는 2진 카운트

#include <avr/io.h>

void delay(unsigned int millisec){	// ms 단위로 딜레이를 주는 함수
	volatile int i,j;
	for (i = 0; i < millisec; i++)
	for (j = 0; j < 1000; j++);
}

int main(void)
{
	DDRA = 0x0F;	// A의 하위 네 비트를 출력 상태로 만든다.
	PORTA = 0x00;	// 출력 상태를 0으로 초기화한다.
	DDRB = 0x00;	// B의 모든 핀을 입력 상태로 만든다.
	PORTB = 0x01;	// B의 0번째 핀을 HIGH로 한다.(내부 풀업 저항 사용 목적)
	
	unsigned char count = 0x00;	// 스위치를 누를 때마다 증가할 카운트 변수
	
	while (1)
	{	
		if (!(PINB & 0x01)){
			if (count == 0x0F) count = 0x00;  
			else count++;
			delay(200);
		}
		PORTA = count & 0x0F;	// 하위 4비트만 취함
	}
}

