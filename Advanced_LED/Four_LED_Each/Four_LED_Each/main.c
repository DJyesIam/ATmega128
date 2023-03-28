// LED 네 개를 각각 다른 주기로 점멸시키는 프로그램

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

uint16_t count = 0;

int main(void)
{
	DDRA |= 0x0F;	// A 포트의 하위 네 비트를 출력 모드로 만든다.
	PORTA &= 0xF0;	// A 포트의 하위 네 비트를 0으로 초기화
	
    while (1) 
    {
		if (!(count % 200)) PORTA ^= (0x01 << 0);
		if (!(count % 100)) PORTA ^= (0x01 << 1);
		if (!(count % 50)) PORTA ^= (0x01 << 2);
		if (!(count % 25)) PORTA ^= (0x01 << 3);
		
		_delay_ms(1);
		count++;
    }
}

