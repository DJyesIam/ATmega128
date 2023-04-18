// 스위치를 누르는 동안 스피커를 켜는 코드

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define M0 0x01 << PORTC0

int main(void)
{
	DDRB &= ~M0;	// PORTB의 0번 포트를 입력 모드로
	PORTB |= M0;	// PORTB의 0번 포트만 출력 HIGH(내부 풀업 저항 사용)
    DDRC |= M0;	// PORTC의 0번 포트만 출력 모드로
    while (1) 
    {
		if (!(PINB & M0)){
			PORTC |= M0;	//On
			_delay_us(250);
			PORTC &= ~M0;	//Off
			_delay_us(250);
		}
    }
}

