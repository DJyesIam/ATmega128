// LED 네 개를 다른 Duty Ratio로 점멸시키는 프로그램

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define PWM_CYCLE 1000	// PWM의 한 주기(1000ms)

uint16_t count = 0, scount = 0;	// scount 변수는 on duty를 잰다.
 
int main(void)
{
	DDRA |= 0x0F;	// A 포트의 하위 네 비트를 출력 모드로 만든다.
	PORTA &= 0xF0;	// A 포트의 하위 네 비트를 0으로 초기화
	
	while (1)
	{
		if (!(count % PWM_CYCLE)){
			PORTA |= 0x0f;
			scount = 0;
		}
		if (scount == 200) PORTA &= ~(0x01 << 0);
		if (scount == 100) PORTA &= ~(0x01 << 1);
		if (scount == 50) PORTA &= ~(0x01 << 2);
		if (scount == 25) PORTA &= ~(0x01 << 3);
		_delay_ms(1);
		count++;
		scount++;
	}
}

