// 폴링을 사용하여 LED를 켜고 끄는 프로그램

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	DDRA |= 0x01 << PORTA0;	// LED
	DDRB &= ~(0x01 << PORTB0); PORTB |= 0x01 << PORTB0;	// 스위치용 포트(내부 풀업 사용)
    while (1) 
    {
		do{
		}while((PINB & 0x01) != 0x00);
		PORTA ^= 0x01 << PORTA0;
		_delay_ms(200);
    }
}

