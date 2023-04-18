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
		if ((PINB & 0x01) == 0x00){		// 스위치가 눌리면
			duty++;		// on_duty를 1 증가시킨다.
			if (duty == 10) duty = 0;	// 만약 밝기가 최대면 밝기를 0으로 한다.
			PORTA = digit[duty];	// 현재의 밝기를 FND로 표시한다.
			_delay_ms(200);		// 디바운싱용 딜레이
		}
		if ((count % PWM_CYCLE) == 0){	// 20ms마다
			PORTC |= 0x01;	// led를 켠다.
			scount = 0;		// scount 초기화
		}
		if (scount == duty) PORTC &= ~(0x01);	
		// scount가 on_duty와 일치하는 순간 led를 끈다.
		// on_duty가 클수록 led를 늦게 끄므로 led의 밝기가 증가하게 된다.
		
		_delay_ms(1);
		scount++;
		count++;
	}
}