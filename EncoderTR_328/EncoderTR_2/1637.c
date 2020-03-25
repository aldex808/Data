#include "1637.h"


uint8_t digits[] =
{
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};



//
void indication(uint16_t n, uint8_t znak)
{
	uint8_t n1, n2, n3;
	n1 =  n % 10;
	n2 = ((n % 100)-n1)/10;
	n3 = ((n % 1000) - n2 - n1) / 100;
	write(digits[n3], digits[n2], digits[n1], znak);
}


//
void write(uint8_t n3, uint8_t n2, uint8_t n1, uint8_t n0)
{
	start();
	writeValue(SetBright); // �������
	start();
	writeValue(0x40); // ������ � �������� �������
	stop();
	start();
	writeValue(0xc0); // ����� � 1-�� �������.
	writeValue(n3);
	writeValue(n2);
	writeValue(n1);
	writeValue(n0);
	stop();
}



// 
void start(void)
{
	DDRD|=(1<<_CLK);
	DDRD|=(1<<_DIO);
	PORTD |= (1<<_CLK);
	PORTD |= (1<<_DIO);
	_delay_us(5);
	PORTD &=~ (1<<_DIO);
	PORTD &=~ (1<<_CLK);
	_delay_us(5);
}

//
void stop(void)
{
	DDRD|=(1<<_CLK);
	DDRD|=(1<<_DIO);
	PORTD &=~ (1<<_CLK);
	PORTD &=~ (1<<_DIO);
	_delay_us(5);
	PORTD |= (1<<_CLK);
	PORTD |= (1<<_DIO);
	_delay_us(5);
}

//
void writeValue(uint8_t value)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		PORTD &=~ (1<<_CLK);
		_delay_us(5);
		if((value & (1 << i)))
		PORTD |= (1<<_DIO);
		else
		PORTD &=~ (1<<_DIO);
		_delay_us(5);
		PORTD |= (1<<_CLK);
		_delay_us(5);
	}
	PORTD &=~ (1<<_CLK);
	_delay_us(5);
	PORTD |= (1<<_CLK);
	_delay_us(5);
}