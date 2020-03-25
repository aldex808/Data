/*
 * display.h
 *
 * Created: 10.01.2018 12:57:57
 *  Author: Администратор2
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>

// data mode: determines how c parameter to shift_in and set_char_at is interpreted
#define ASCII 0    // interpreted as ascii
#define SEGMENTS 1 // interpreted as segments to turn on/off

void display_init(void); // инициализаци¤ диспле¤
uint8_t set_brightness(uint8_t b);//устаовка ¤ркости
void shift_in(char c);// сдвиг символа
void set_char_at(char c, uint8_t offset);//вывод символа в оределенное знакоместо
void set_segments_at(uint8_t segments, uint8_t offset);// вывод сегмента в определенное знакоместо
void shift_in_segments(uint8_t segments); // сдвиг сегмента
void set_number(uint16_t num); // вывод цифр
void clear_screen(void); // очистить дисплей
void set_time(uint8_t hour, uint8_t min, uint8_t sec); // вывод времени


// Segment A: PB0
#define A_DDR  DDRB
#define A_PORT PORTB
#define A_BIT  PB0
#define A	0
// Segment B: PB1
#define B_DDR DDRB
#define B_PORT PORTB
#define B_BIT  PB1
#define B	1
// Segment C: PB2	
#define C_DDR DDRB
#define C_PORT PORTB
#define C_BIT  PB2
#define C	2
// Segment D: PB3
#define D_DDR DDRB
#define D_PORT PORTB
#define D_BIT  PB3
#define D	3
// Segment E: PB4
#define E_DDR DDRB
#define E_PORT PORTB
#define E_BIT  PB4
#define E	4
// Segment F: PB5
#define F_DDR DDRB
#define F_PORT PORTB
#define F_BIT  PB5
#define F	5
// Segment G: PB6
#define G_DDR DDRB
#define G_PORT PORTB
#define G_BIT  PB6
#define	G	6
// Segment DP: PB7
#define DP_DDR DDRB
#define DP_PORT PORTB
#define DP_BIT  PB7
#define	DP	7

// Digit 1: PC1
#define DIGIT1_DDR DDRC
#define DIGIT1_PORT PORTC
#define DIGIT1_BIT  1
// Digit 2: PC2
#define DIGIT2_DDR DDRC
#define DIGIT2_PORT PORTC
#define	DIGIT2_BIT	2
// Digit 3: PC3
#define DIGIT3_DDR DDRC
#define DIGIT3_PORT PORTC
#define DIGIT3_BIT	3
// Digit 4: PC4
#define DIGIT4_DDR DDRC
#define DIGIT4_PORT PORTC
#define	DIGIT4_BIT	4





#endif /* DISPLAY_H_ */