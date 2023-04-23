// 인터럽트를 사용하여 LED를 켜고 끄는 프로그램2

#include <avr/io.h>
#include <avr/interrupt.h>
#define Use_IE0_Interrupt (EIMSK |= (0x01 << INT0))		// PORTD0를 인터럽트 핀으로 사용
#define IE0_Detect_Falling EICRA &= ~(0x01 << ISC00)	// 하강엣지
#define IE0_Detect_Rising (EICRA |= ((0x01 << ISC01) | (0x01 << ISC00)))	// 상승엣지

ISR(INT0_vect){
	if ((EICRA & (0x01 << ISC00)) == 0){
		PORTA |= (0x01 << PORTA0);
		IE0_Detect_Rising;
	}
	else {
		PORTA &= ~(0x01 << PORTA0);
		IE0_Detect_Falling;
	}
}

void initialize(void){
	DDRA |= (0x01 << PORTA0);	// LED
	DDRD &= ~(0x01 << PORTD0); PORTD |= (0x01 << PORTD0);	// 스위치용 포트(내부 풀업 사용)
	
	IE0_Detect_Falling;			// 하강 엣지
	Use_IE0_Interrupt;			// 인터럽트 활성
	sei();						// 전역 인터럽트
}

int main(void)
{
	initialize();
	while (1);
}

