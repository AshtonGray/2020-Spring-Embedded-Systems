/*
 * Project1GoodVersion.c
 *
 * Created: 2/10/2020 2:25:47 PM
 * Author : Ashton, brad
 */ 

#include <avr/io.h>

// Test 8 Final
// note: brad is awful at spelling 

int main(void)
{
	// ---- initialize outputs and variables ----- //
	DDRB = (1<<DDB3) | (1<<DDB5) | (1<<DDB2); // output for MOSI, SCK, and SS
	DDRC = (1<<DDC0) | (1<<DDC1) | (1<<DDC2) | (1<<DDC3); // output C
	SPCR = (1<<SPIE) | (1<<SPE) | (1<<MSTR) | (0<<SPR0); //changing clock will effect speed pf output 
	SPSR = (1<<SPI2X);
	
	uint8_t number_map[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111}; // 0-9
	
	uint8_t minutes = 0;
	uint8_t seconds_tens = 0;
	uint8_t seconds_ones = 0;
	uint8_t tenths = 0;


	// ----- timing ----- //
	
	TCCR1B = 0b00000010;// set pre-scaler value (for clock)
	TCNT1 = 0; // clock counter, important to note TCNT1 cant get bigger than 2^16 (65536)(16 bits)
	uint16_t interval = 25000; // how many clock cycles in 1/10th of a second, 
	uint16_t pause = 400; // cannot be larger than 2^16 bc of how it works with TCNT1 (16 bits)
	uint16_t t_hold; // value to use in hold 
	
	char stopped = 1; // if stopped is odd time will not count, if it is even it will 
	
	while(1)
	{
	
		// display minutes
		SPDR = number_map[minutes]; // loading data register
		PORTC = 0b00000001;
		PORTB = 0b00000000; // SS low, start sending
		while(!(SPSR & (1<<SPIF))); // sends until stops data transfer
		PORTB = 0b00000100; // SS High, stop sending
		
		t_hold = TCNT1; // pause 
		while(pause + t_hold> TCNT1);
		
		// seconds (tens) 
		SPDR = number_map[seconds_tens];
		PORTC = 0b00000010;
		PORTB = 0b00000000; // SS low, start sending
		while(!(SPSR & (1<<SPIF))); // stops data transfer
		PORTB = 0b00000100; // SS High, stop sending
		
		t_hold = TCNT1;
		while(pause + t_hold> TCNT1);
		
		// seconds (ones)
		SPDR = number_map[seconds_ones] + 0b10000000; // 2^7 to put in the decimal 
		PORTC = 0b00000100;
		PORTB = 0b00000000; // SS low, start sending
		while(!(SPSR & (1<<SPIF))); // stops data transfer
		PORTB = 0b00000100; // SS High, stop sending
		
		t_hold = TCNT1;
		while(pause + t_hold> TCNT1);
		
		// tenths 
		SPDR = number_map[tenths];
		PORTC = 0b00001000;
		PORTB = 0b00000001; // SS low, start sending 
		while(!(SPSR & (1<<SPIF))); // stops data transfer
		PORTB = 0b00000100; // SS High, stop sending
		
		t_hold = TCNT1;
		while(pause + t_hold> TCNT1);
		
		//reset 
		PORTD = 0b00001001; // pd3
		if(PIND == 0b00000001){
			minutes = 0;
			seconds_tens = 0;
			seconds_ones = 0;
			tenths = 0; 
			stopped = 0;
		}
		
		//pause 
		PORTD = 0b00001001; // pd0
		if(PIND == 0b000001000){
			stopped ++;
			t_hold = TCNT1;
			while(t_hold + 30000 > TCNT1); // prevent double pressing 
		}
		
		if(TCNT1 > interval && stopped % 2 == 1){
			TCNT1 = 0; // if this wasn't here TCNT1 could over flow causing minor issues 
		}
		
		if(TCNT1 > interval && stopped % 2 == 0){
			TCNT1 = 0;
			tenths++; // increase data
			if (tenths >= 10){ // reset to 0 if 10
				tenths = 0;
				seconds_ones ++; 
				if (seconds_ones >= 10){
					seconds_ones = 0;
					seconds_tens++;
					if (seconds_tens >= 6){
						seconds_tens = 0;
						minutes ++; 
						if (minutes >= 10){
							minutes = 0;
						}
					}
				}
			}
		}
	}
}

	
