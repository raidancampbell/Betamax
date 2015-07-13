/*
 * Betamax.c
 *
 * Created: 12/28/2014 4:03:55 PM
 *  Author: Aidan
 */ 
#define F_CPU 8000000UL
#include <delay.h>

#include <avr/io.h>
#include "t2313_softpwm.h"
#include "virtuabotixRTC.h"
#include "Betamax.h"

extern unsigned char compbuff[];

void set_leds(int led_mask){
	PORTD = led_mask & 0xFF;
	led_mask <<= 8;
	PORTB = led_mask & 0xF;
}

int main(void)
{
	
	//  IO setting of ports
	DDRB = 0x0F;
	DDRC = 0b00100000;
	DDRD = 0xFF;
	int MAX_BRIGHTNESS = 12;
	Init(); //this is initializing the PWM
	Init_RTC(); //this is initializing the RTC
	//setDS1302Time(0,39,13,1,4,1,2015);
	delay_us(10);//make sure the time is updated before reading
	updateTime();
	setAllBrightness(MAX_BRIGHTNESS);
    while(1)
    {//update time, set first 8 strips to binary representation of current seconds
		updateTime();
		uint8_t tempSeconds = seconds;
		for(int i=0;i<=7;i++){
			tempSeconds = seconds; //because the right shift is permanent
			//compbuff[i] = ((tempSeconds >> i) & 0x01)*MAX_BRIGHTNESS; 
			//either on or off, depending on if the bit is set
		}
		compbuff[0] = 1;
		compbuff[1] = 0;
		compbuff[2] = 254;
		compbuff[3] = 255;
		delay_ms(500);
    }
}

void setAllBrightness(int newBrightness){
	for(int i=0 ; i<CHMAX ; i++) {
		compbuff[i] = newBrightness;
	}
}

void setIndividualBrightness(int newBrightness, int zoneNumber){
	if (zoneNumber >= CHMAX) return;
	compbuff[zoneNumber] = newBrightness;
}