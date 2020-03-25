/*
 * Message.c
 *
 * Created: 10.01.2018 23:49:52
 *  Author: Администратор2
 */ 

#include "Message.h"
void Message_out()
	{
		set_brightness(80);   // начальная установка яркости дисплея
		display_init();        // инициализация дисплея

		shift_in(' '); // выводим бегущую строку " ON  "
		_delay_ms(50);
		shift_in('O');
		_delay_ms(50);
		shift_in('N');
		_delay_ms(50);
		shift_in(' ');
		_delay_ms(300);
		clear_screen();// очистка дисплея
		_delay_ms(100);
	}
	void Message_out2(){
		clear_screen();
		_delay_ms(100);
		shift_in('s'); // выводим бегущую строку "set"
		_delay_ms(50);
		shift_in('E');
		_delay_ms(50);
		shift_in('t');
		_delay_ms(300);
		clear_screen();
		_delay_ms(100);
		
	}