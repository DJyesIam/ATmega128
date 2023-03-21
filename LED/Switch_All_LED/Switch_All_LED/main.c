// 스위치를 누를 때마다 네 개의 LED를 한 번에 껐다 킨다.

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	DDRA = 0x0F;	// A의 하위 네 비트를 출력 상태로 만든다.
	PORTA = 0x00;	// 출력 상태를 0으로 초기화한다.
	DDRB = 0x00;	// B의 모든 핀을 입력 상태로 만든다.
	PORTB = 0x01;	// B의 0번째 핀을 HIGH로 한다.(내부 풀업 저항 사용 목적)
	
	unsigned char isON = 0;
	while (1)
	{
		if (!(PINB & 0x01)){
			if (isON){
				 PORTA &= 0xF0;	
				 isON = 0;
			}
			else{
				 PORTA |= ((PORTA & 0xF0) | 0x0F);
				 isON = 1;
			}
		}
		_delay_ms(200);
	}
}

