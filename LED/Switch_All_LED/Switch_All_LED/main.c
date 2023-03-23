// 스위치를 누를 때마다 네 개의 LED를 한 번에 껐다 킨다.

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	// DDRA |= 0x0F;	// PORTA의 하위 네 비트를 출력 상태(1)로 만든다.
	DDRA |= ((1<<PORTA0) | (1<<PORTA1) | (1<<PORTA2)| (1<<PORTA3));
	// 위와 같은 코드. 핀이 나중에 바뀌었을 때 16진수를 16진수를 계산할 필요가 없다. 다른 부분에도 적용 가능
	PORTA &= !0x0F;	// PORTA의 하위 네 비트를 0으로 초기화한다.
	DDRB &= !0x01;	// PORTB의 0번 핀을 입력 상태(0)로 만든다.
	PORTB |= 0x01;	// PORTB의 0번째 핀을 1로 만든다.(내부 풀업 저항 사용 목적)
	
	unsigned char isON = 0;		// LED들이 켜져있는지 저장하는 변수
	
	while (1)
	{
		if (!(PINB & 0x01)){	// 스위치가 눌리면
			if (isON){	// LED가 다 켜져 있으면
				 PORTA &= 0xF0;		// LED를 다 끈다.
				 isON = 0;	
			}
			else{	// LED가 다 꺼져 있으면
				 PORTA |= ((PORTA & 0xF0) | 0x0F);	// LED를 다 킨다.
				 isON = 1;
			}
		}
		_delay_ms(200);
	}
}

