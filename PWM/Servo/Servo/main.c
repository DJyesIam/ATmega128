#include <avr/io.h>
#define BOT (PIND & 0x01)
#define TC0_FAST_PWM (1 << WGM00| 1 << WGM01)			// 웨이브 제너레이션 모드 : 고속 PWM 모드(11)
#define TC0_NONIVERT_PWM (1 << COM01)						// 비교출력모드 : BOTTOM SET, OCR0 CLEAR(10)
#define TC0_PRESCALE_1024 (1 << CS00 | 1 << CS01 | 1 << CS02)	// 프리스케일 1024(111)

unsigned int prev_BOT = 1, count = 0;

int main(void)
{
	DDRB = (1 << PORTB4);
	DDRD = 0x00; PORTD|=1 << PORTD0; // Pull-Up
	OCR0 = 23; count = 23;
	TCCR0 |= TC0_FAST_PWM | TC0_NONIVERT_PWM | TC0_PRESCALE_1024;
	
    while (1) 
    {
		if ((BOT == 0) && (prev_BOT == 1)){
			count = count + 2;
			count = (count < 39) ? count: 9;
			OCR0 = count;
		}
		prev_BOT = BOT;
    }
}

