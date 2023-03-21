// 스위치가 눌린 상태이면 4개의 LED를 한 번에 켜고 아니면 끈다.


#include <avr/io.h>


int main(void)
{
	DDRA = 0x0F;	// A의 하위 네 비트를 출력 상태로 만든다.
	PORTA = 0x00;	// 출력 상태를 0으로 초기화한다.
	DDRB = 0x00;	// B의 모든 핀을 입력 상태로 만든다.
	PORTB = 0x01;	// B의 0번째 핀을 HIGH로 한다.(내부 풀업 저항 사용 목적)
	
    while (1) 
    {
		if (!(PINB & 0x01)) PORTA |= 0x0F;
		else PORTA &= ~(0x0F);
    }
}

