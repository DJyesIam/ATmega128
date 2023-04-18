// tone 함수 만들어서 사용하기

#include <avr/io.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__	// 딜레이 함수에 변수 사용 가능
#include <util/delay.h>
#define M0 1 << PORTC0

void tone(int fr, int duration){
	int tms = 500/fr, i;
	for (i = 0; i<duration; i++){
		PORTC |= M0;
		_delay_ms(tms);
		PORTC &= ~M0;
		_delay_ms(tms);
	}
}

int main(void)
{
	DDRB &= ~M0;	// PORTB의 0번 비트만 입력 모드로
	PORTB |= M0;	// PORTB의 0번 비트만 출력 HIGH(내부 풀업 저항 사용)
	DDRC |= M0;	// PORTC의 0번 포트만 출력 모드로
	while (1) 
	{
		if (!(PINB & M0)){
			tone(500,500);
			tone(250,500);
		}
	}
}

