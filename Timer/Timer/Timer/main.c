#include <avr/io.h>
#include <avr/interrupt.h>

void disp_digit(unsigned char, unsigned char);

unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
volatile unsigned int count = 0;

ISR(TIMER0_OVF_vect){
	count++;
	TCNT0 = 6;
}

int main(void)
{
	unsigned char num;
	DDRC = 0xff; DDRG |= 0x0f;	// 4자리 FND
	TCCR0 |= 0x01 << CS02;	// Prescale 64  4us
	TIMSK |= 1<<TOIE0;		// Interrupt Enable	TCNT0=6;				// 4us x (256 - 6) = 1ms	sei();
	
    while (1) 
    {
		if ((count % 10)==0){ //1자리
			num= (count/10)%10;
			disp_digit(num, 3);
		}
		if ((count % 10)==2){ //10자리
			num= (count/100)%10;
			disp_digit(num, 2);
		}
		if ((count % 10)==4){ //100자리
			num= (count/1000)%10;
			disp_digit(num, 1);
		}
		if ((count % 10)==8){ //1000자리
			num= (count/10000)%10;
			disp_digit(num, 0);
		}
    }
}

void disp_digit(unsigned char num, unsigned char d){
	PORTC = digit[num];
	PORTG = 0x01 << d;
}

