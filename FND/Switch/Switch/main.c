// 스위치를 누를 때마다 7 Segment에서 0에서 9까지 켜는 프로그램

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

int main(void)
{
	uint8_t digit[] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
	uint16_t num = 0;
	
	DDRA = 0xFF;
	PORTA = digit[num];
	DDRB &= ~(0x01); PORTB |= 0x01;
	while (1)
	{
		if ((PINB & 0x01) == 0x00){
			PORTA = digit[++num % 10];
			_delay_ms(200);
		}	
	}
}