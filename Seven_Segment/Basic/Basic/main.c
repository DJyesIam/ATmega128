// 0.2초 간격으로 7 Segment에서 0에서 9까지 켜는 프로그램

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

int main(void)
{
	uint8_t digit[] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
	uint16_t count = 0, num = 0;
	
	DDRA = 0xFF;
    while (1) 
    {
		PORTA = digit[num];
		num = count++ % 10;
		_delay_ms(200);
		
    }
}

