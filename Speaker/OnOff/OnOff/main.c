// 스위치를 누를 때마다 스피커를 켰다 끄는 코드

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define M0 1<<0

int main(void)
{
	DDRB = 0x00;	// PORTB의 모든 포트를 입력 모드로
	PORTB = 0x01;	// PORTB의 0번 포트만 출력 HIGH(내부 풀업 저항 사용)
    DDRC |= M0;	// PORTC의 0번 포트만 출력 모드로
    while (1) 
    {
		if (!(PINB & M0)){
			PORTC |= M0;	//On
			_delay_us(500);
			PORTC &= ~M0;	//Off
			_delay_us(500);
		}
    }
}

