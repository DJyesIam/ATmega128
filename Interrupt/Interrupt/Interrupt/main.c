// 인터럽트를 사용하여 LED를 켜고 끄는 프로그램

#include <avr/io.h>
#include <avr/interrupt.h>
#define Use_IE0_Interrupt (EIMSK |= (0x01 << INT4))
#define IE0_Detect_Falling (EICRB |= (0x01 << ISC41))

ISR(INT4_vect){
	PORTA ^= 0x01 << PORTA0;
}

int main(void)
{
    DDRA |= 0x01 << PORTA0;	// LED
    DDRE &= ~(0x01 << PORTE4); PORTE |= 0x01 << PORTE4;	// 스위치용 포트(내부 풀업 사용)
	
	IE0_Detect_Falling;
	Use_IE0_Interrupt;
	sei();						// 전역 인터럽트
    while (1);
}

