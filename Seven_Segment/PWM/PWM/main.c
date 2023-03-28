// 스위치를 누를 때마다 LED의 밝기를 바꾸는 프로그램

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#define PWM_CYCLE 20

int main(void)
{
	uint8_t digit[] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
	uint16_t count = 0, scount = 0, duty = 0;
	
	DDRA = 0xFF;
	PORTA = digit[duty];
	DDRB &= ~(0x01); PORTB |= 0x01;
	DDRC |= 0x01;
	
	while (1)
	{
		if ((PINB & 0x01) == 0x00){
			duty++;
			if (duty == 10) duty = 0;
			PORTA = digit[duty];
			_delay_ms(200);
		}
		if ((count % PWM_CYCLE) == 0){
			PORTC |= 0x01;
			scount = 0;
		}
		if (scount == duty) PORTC &= ~(0x01);
		
		_delay_ms(1);
		scount++;
		count++;
	}
}