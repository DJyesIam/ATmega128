// tone 함수 만들어서 사용하기

#include <avr/io.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#define M0 1<<0

void tone(int fr, int duration){
	int tms = 500/fr, i;
	for (i = 0; i<duration; i++){
		PORTC |= M0;
		_delay_us(tms);
		PORTC &= ~M0;
		_delay_us(tms);
	}
}

int main(void)
{
	int i;
	DDRB = 0x00;	// PORTB의 모든 포트를 입력 모드로
	PORTB = 0x01;	// PORTB의 0번 포트만 출력 HIGH(내부 풀업 저항 사용)
	DDRC |= M0;	// PORTC의 0번 포트만 출력 모드로
	while (1) 
	{
		if (!(PINB & M0)){
			for (i = 0; i < 20; i++){
				tone(320,8);
				tone(480,12);
			}
		}
	}
}

