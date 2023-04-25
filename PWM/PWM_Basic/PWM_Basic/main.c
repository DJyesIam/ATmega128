#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define TC0_FAST_PWM (1 << WGM01 | 1 << WGM00)
#define TC0_NONINVERT_PWM (1 << COM01)
#define TC0_PRESCALE_32 (1 << CS01 | 1 << CS00)

void disp_FND(unsigned char num);
void initialize(void);
void disp_digit(unsigned char num, unsigned char d);
unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};

int main(void)
{
	unsigned int count = 0;
	unsigned char duty;
	initialize();
	
    while (1) 
    {
		duty = count % 256;
		OCR0 = duty;
		count++;
		disp_FND(duty);	
    }
}

void disp_FND(unsigned char num){
	unsigned char fnd[4];
	fnd[3] = (num/1000)%10;
	fnd[2] = (num/100)%10;
	fnd[1] = (num/10)%10;
	fnd[0] = num%10;
	for (int i=0; i<4; i++){
		disp_digit(fnd[i],i);
		_delay_ms(2);
		if(i%2) _delay_ms(1);
	}
}

void disp_digit(unsigned char num, unsigned char d){
	PORTC = digit[num];
	PORTG = 1<<d;
}
void initialize(){
	DDRB |= 1 << PORTB4; 
	DDRC=0xFF;
	DDRG=0x0F;
	TCCR0 |= TC0_FAST_PWM | TC0_NONINVERT_PWM | TC0_PRESCALE_32;
}

