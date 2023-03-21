// 스피커를 1KHz로 켜는 코드

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define M0 1<<0

int main(void)
{
	DDRC |= M0;	// PORTC의 0번 포트만 출력 모드로
	while (1)
	{
		PORTC |= M0;	//On
		_delay_us(500);
		PORTC &= ~M0;	//Off
		_delay_us(500);
	}
}



