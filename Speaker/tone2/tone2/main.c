// tone2 함수 만들어서 사용하기

#include <avr/io.h>
#include <stdint.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#define HALF_CYC 500000UL
#define M0 1<<0

void tone2(int fr, int duration){
	uint32_t t_tot = HALF_CYC/fr;
	int t_ms = t_tot / 1000;
	int t_us = t_tot % 1000;
	int i;
	for (i = 0; i < duration; i++){
		PORTC |= M0;
		_delay_ms(t_ms); _delay_us(t_us);
		PORTC &= ~M0;
		_delay_ms(t_ms); _delay_us(t_us);
	}
}

int main(void)
{
	int i;
	int freq[] = {523, 587, 659, 698, 784, 880, 988, 1047};
	DDRB = 0x00;	// PORTB의 모든 포트를 입력 모드로
	PORTB = 0x01;	// PORTB의 0번 포트만 출력 HIGH(내부 풀업 저항 사용)
	DDRC |= M0;		// PORTC의 0번 포트만 출력 모드로
	while (1)
	{
		if (!(PINB & M0)){
			for (i = 0; i < 8; i++){
				tone2(freq[i], 300);
				_delay_ms(500);
			}
		}
	}
}

