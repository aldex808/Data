/*
 * encoder_tr.c
 * Chip Atmega328
 * Created: 05.03.2020 15:21:03
 *  Author: aldex
 */ 


//  ��������� ������������ � ������� ��������
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/eeprom.h>

#include "button_lib.h"
#include "1637.h"
#include "ADC.h"
#include "Timer_init.h"
//============== ���������� ������� ======================
#define LED_STATUS	PD5 //D10

#define RELAY_K9		PD7//D9
#define MAX_VOLT		255
#define MAX_VOLT10		250
#define MIN_VOLT	     0
#define TRUE			1
#define FALCE			!TRUE
//=============================================================== ��������� ������� ========================================
//���������
#define EncoderDDR DDRD                        /*���� ���������*/
#define EncoderPORT PORTD                    /*���� ����������*/
#define EncoderPIN PIND                        /*���� ������*/
#define EncoderF1 (1<<0)                    /*���� ������ ��������*/
#define EncoderF2 (1<<2)                    /*���� ������ ��������*/
//=================================== ���������� ��� �������� ============================================
//����������� ����������
unsigned int EncoderValue = 0;

enum { State0, StateA, StateB, StateAB };     //��������� ��������
	void To_Do_Step_Up();
	void To_Do_Step_Dn();

//����������
static char EncPrev;                          //���������� ��������� ��������
static char EncPrevPrev;                      //����-���������� ��������� ��������


volatile unsigned char voltage_v,flag, button = 0;
unsigned int  Urms=0;
unsigned long lRez_long=0;

//=========================================== ������������� ������� =======================
//������������� ��������
void EncoderInit(void)
{
	EncoderDDR &= ~(EncoderF1 | EncoderF2); //��������� ������ �� ����
	EncoderPORT |= EncoderF1 | EncoderF2;   //��������� ������������� ����������
	EncPrev = State0;                       //������������� ����������� ���������
	EncPrevPrev = State0;                   //������������� ����-����������� ���������
	
}
//=============================================================================================
//================================== ������� ��������� �������� ========================
//��������� ��������
void EncoderExe(void)
{
	char EncCur = 0;

	if(! (EncoderPIN & EncoderF1))
	EncCur  = StateA; //����� ���� 1 ��������

	if(! (EncoderPIN & EncoderF2))
	EncCur |= StateB; //����� ���� 2 ��������

	if(EncCur != EncPrev) {                    //���� ��������� ����������,
		if(EncPrev == StateAB && EncCur != EncPrevPrev ){//���� ���������� ��������� StateAB � ������� � ����-���������� �� �����,
			if(EncCur == StateB) 
			{ 
				
				 To_Do_Step_Up();
			}             //���� ������� ��������� StateB,
			
			
			else
			{
				            //��� ����
				
				To_Do_Step_Dn();
			}
			
			
		}
		EncPrevPrev = EncPrev;              //���������� ����-����������� ���������
		EncPrev = EncCur;                   //���������� ����������� ���������
	}
}
//===========================================================================
//=======================================    =================================
void To_Do_Step_Up()
{
	
	if(EncoderValue<MAX_VOLT10 && flag == FALCE) EncoderValue+=10;
	 if(EncoderValue<MAX_VOLT && flag == TRUE) EncoderValue++;
	
}
void To_Do_Step_Dn()
{
	
	if(EncoderValue > MIN_VOLT && flag == FALCE) EncoderValue-=10 ;
	 if(EncoderValue > MIN_VOLT && flag == TRUE) EncoderValue--;
}
//======================================================================================

//===================================================================================================================
int main (void)
{
	// ����� �����/������
	DDRB = 0xFF; // ������
	PORTB = 0x00;
	DDRD |=  (1 << LED_STATUS) | (1 << RELAY_K9); // ����� //PD6 PD7 LED_STATUS ������
	//PORTD |= (1 << ENC_A)|(1 << ENC_B); //| (1 << KEY_ED); // ���������� ������������� ���������
	
	// ������ 0
	//TCCR0B |= (1 << CS00); // ������������ �0 ��� ������������
	//TIMSK0 |= (1 << TOIE0); // ��������� ���������� �� ������������ �0
	
	// ================================������� ����������=========================
//	MCUCR |= (1 << ISC01); // ���������� �� ������� ������ INT0(�� ����� ��������)
//	EIFR |= (1 << INTF0); // ������� ���� �������� ����������
//	EIMSK |= (1 << INT0);  // ��������� ������� ���������� INT0
	
	//===========================��������� ������� �� ���������� � �������� 100��=====================
	OCR2A = F_CPU / 1024 / 100;					//������� ��� ��������� ������� 100��
	TIMSK2 |= (1<<OCIE2A);						//���������� ��� ����������
	TCCR2B = (1<<CS22)| (1<<CS21)| (1<<CS20);	//������������ �� 1024
	TCCR2A = (1<<WGM21)| (0<<WGM20);				//����� CTC (����� ��� ����������)
	//=========================================
	Timer_init();
	EncoderInit();
	Adc_rms_init();
	fn_SetMeas();
	BtnInit();
	Urms = 0;
	sei(); // ��������� ��������� ����������
	
	while(1)
	{
		
		 
		if(!sMeasure.cReady)

		{
			//Rashet_rms();
			sMeasure.iRez= fn_Calculate();
			lRez_long=(unsigned long)sMeasure.iRez * U_OPORN;
			lRez_long*=KF;
			lRez_long+=512; // ������� ������ �������������� �������
			lRez_long/=1024; // ����� � ��������� �������
			lRez_long+=500;// ������� ������ �������������� �������
			Urms=(unsigned int)(lRez_long/1000); // ��������� � ������
			
			//set_number(Urms);//   ����� �������� � ������� ���
			//set_char_at('U', 0);//����� ������� � ����������� ����������
			indication(Urms, 0x62);
			//
			_delay_ms(10);
			
			fn_SetMeas();
		}
		
		uint8_t button1 = BtnGet();
		//if((PIND & (1 << BUTTON_ENC)) == 0) // ���� ������ ������ ��������
		if (button1 == BTN_SHRT2)
		{
			//PORTD |= (1 << PD7);
			//button++;
			if(button++>= 2) {
			 button = 1;
			 
			}
		}
			//while((PIND & (1 << BUTTON_ENC)) == 0){} // ���� ���������� ������
			//_delay_ms(50);
			switch(button) {
				
		case 1:	 PORTD |= (1 << LED_STATUS); // 
			 PORTD |= (1 << RELAY_K9);
			// clear_screen();
			 TIMSK1 &=~ (1<<TOIE1);//���������� ���
			 flag = FALCE;
			
			EncoderExe();
			
			 voltage_v =  EncoderValue;
			 if(voltage_v > 255) EncoderValue = 255;
				indication(voltage_v, 0xf3);
				_delay_ms(10);
			//button = 0;
			break;		
			
			//else PORTD &= ~(1 << PD7);
		case 2:
			 PORTD &=~ (1 << LED_STATUS); //
			 flag = TRUE;
			 
			EncoderExe();
			 voltage_v =  EncoderValue;
			 indication(voltage_v, 0xf3);
			_delay_ms(10);
			
			break;
			default:
			break;
			}
		
			if (button1 == BTN_LONG2)
			{
				
				PORTD &=~(1 << LED_STATUS);
				flag =FALCE;
				
				indication(voltage_v,0x60);
				//voltage_v = 255;
				PORTB = voltage_v;
				_delay_ms(3000);
				PORTD &=~(1 << RELAY_K9);
				TIMSK1 |= (1<<TOIE1);//������ ���
				indication(Urms, 0x62);
			button = 0;
			_delay_ms(100);
			
			}
			
		
		
	}//  while
}// main

ISR (TIMER2_COMPA_vect)							//interrupt 100hz
{
	BtnExe();
	//PORTD ^=( 1 << LED_STATUS );
}
 