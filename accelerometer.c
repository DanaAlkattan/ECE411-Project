/*
 * GccApplication14.c
 *
 * Created: 11/24/2017 5:49:10 PM
 * Author : billy
 */ 

/*
 *
 * Created: 11/23/2017 1:00:23 AM
 * Author : billy
 */ 

#include<avr/io.h>
#include<time.h>
#include<math.h>
#include<util/delay.h>
#include<avr/sfr_defs.h>

//static int offsets [] = {0, 0, 0};		//global offsets
static int _average = 10;				//number of iterations to find averages


//reference voltage for device
#define VOLTAGE 3.3

//ADC channels for accelerometer input
#define X_CHANNEL 0x45
#define Y_CHANNEL 0x44
#define Z_CHANNEL 0x43

//#ifndef F_CPU
//#define F_CPU 1600000UL 
//#endif


int init_ADC();
int make_orientation();
int make_xy(int to_make);
int _mapMMA7361G(int value);
void setOffSets(int xOffSet, int yOffSet, int zOffSet);
int getXVolt();
int getYVolt();
int getZVolt();
int getXRaw();
int getYRaw();
int getZRaw();
int _mapMMA7361V(int value);
void calibrate();
int getOrientation();
int getXAccel();
int getYAccel();
int getZAccel();
int ATmap(int to_map, int in_min, int in_max, int out_min, int out_max);


//maps function from one range to another. copied from arduino library and functions exactly like arduino map, except takes int parameters and
//returns int
int ATmap(int to_map, int in_min, int in_max, int out_min, int out_max)
{
	return (to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//set up inputs and Analog-Digital converter
int init_ADC()
{
	//disable digital input for ADC0, 1, 2, 3, 4, 5
	//DIDR0|=0x3F;
	// set Vref = AVCC
	ADMUX = 0x60;
	ADMUX |= (1<<REFS0);
	ADMUX |= (1<<ADLAR);
	// set resolution to 10 bits
	//ADMUX &= (0<<ADLAR);
	//set prescalar to 16 and enable ADC
	ADCSRA = 0x80;
	ADCSRA |= (1<<ADPS2)|(1<<ADEN);
	//ADCSRA &= ~(1<<ADPS1) & ~(1<<ADPS0);
	//set to free running
	//ADCSRA &=  (1<<ADTS2) & (1<<ADTS1) & (1<<ADTS0);
	
	return 1;
}





//raw accelerometer reading. adds offsets calculated in calibration
int getXRaw()
{
	int low = 0;
	int high = 0;
	//set ADC channel to x channel
	ADMUX &= 0xE0;
	ADMUX |= X_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	
	
	ADCSRA|=(1<<ADIF);
	//read ADC
	low |= ADCL;
	high |= ADCH;
	//x |= ADCL;
	//x |= (ADCH << 8);
	_delay_us(1000);
	return (high * 4); //+ offsets[0]+2;
}


//raw accelerometer reading. adds offsets calculated in calibration
int getYRaw()
{
	int high = 0;
	int low = 0;
	//int y = 0;
	//set ADC channel to x channel
	ADMUX &= 0xE0;
	ADMUX |= Y_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	//read ADC
	low |= ADCL;
	high |= ADCH;
	
	_delay_us(1000);
	return (high * 4);
}


//raw accelerometer reading. adds offsets calculated in calibration
int getZRaw()
{
	int z = 0;
	//set ADC channel to z channel
	ADMUX &= 0xE0;
	ADMUX |= Z_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	//read ADC
	//z |= ADCL;
	z |= ADCH;
	
	
	_delay_us(1000);
	
	return z;
}



//get average orientation from 10 readings then put through
//threshold of 10% from center
//returns -1 = left
// 1 = right
// -2 = down
// 2 = up
int make_orientation()
{
  int i;
  int gemiddelde = 10;
  int x = 0;
  int y = 0;
  int z = 0;
  
  for(i = 0; i<gemiddelde ; i++)              //We take in this case 10 measurements to average the error a little bit
  {
    x = x+getXRaw();
    y = y+getYRaw();
    z = z+getZRaw();
  }
  x= x/gemiddelde;
  y = y/gemiddelde;
  z = z/gemiddelde;

  
	if(x > 624)
		return -1;
		 
	if(y > 624)
		return -2;
		
	if(x < 400)
		return 1;
	
	if(y < 400)
		return 2;

	return 0;
}




int main()
{
	int direction;
	int x = 0;
	int y = 0;
	int z = 0;
	
	//initialize button input at PB1
	//DDRB &= 0xFE; //(0 << DDB0);
	//DDRD &= 0xF8;
	
	//set pullup resistor
	//PORTB |= (1 << PORTB1);
	//PORTB |= (1 << PORTB0);
	
	//initialize accelerometer inputs
	init_ADC();
	
	//initialize outputs
	DDRD = 0xFF;
	DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD6) | (1 << DDD5);
	DDRB |= (1 << DDB7) | (1 << DDB6) | (1 << DDB0);
	 
	//PORTD = 0x0;
	//PORTB = 0;
	
	PORTD = 0xFF;
	PORTB = 0xFF;
	
	_delay_ms(10000);
	
	PORTD = 0x0;
	PORTB = 0x0;
	
	_delay_ms(1000);
	//PIND |= (1<<PIND0);

	while(1)
	{
		
		
		
		//loop until button press
		if(1)
		{
			//100 us delay
			_delay_ms(1000);
			
			
			
			//get direction from accelerometer
			direction = make_orientation();
			
			/*
			PORTD &= (0 << PINB7);
			PORTD &= (0 << PIND6);
			PORTD &= (0 << PIND5);
			PORTB |= (1 << PINB0);
			*/
			//PORTD = 0xFF;
			//PORTB = 0xFF;
			
			_delay_ms(1000);
			/*
			PORTB &= (0 << PINB0);
			
			PORTD &= (0 << PIND6);
			PORTD &= (0 << PIND5);
			PORTD |= (1 << PINB7);
			
			*/
			PORTD = 0x0;
			PORTB = 0x0;
			
			_delay_ms(1000);
			/*
			PORTB &= (0 << PINB0);
			PORTD &= (0 << PINB7);
			
			PORTD &= (0 << PIND5);
			PORTD |= (1 << PIND6);
			
			_delay_ms(1000);
			
			PORTB &= (0 << PINB0);
			PORTD &= (0 << PINB7);
			PORTD &= (0 << PIND6);
			PORTD |= (1 << PIND5);
			
			_delay_ms(1000);
			
			
			*/
			//activate indicated lights, deactivate other lights
			switch(direction) {
				//up
				case(-1): {
					
					PORTB &= (0 << PINB0);
					
					PORTD &= (0 << PIND6);
					PORTD &= (0 << PIND5);
					PORTB |= (1 << PINB7);
					//PORTD = 0xFF;
					//PORTB = 0;
					break;
				}
				//down
				case(1): {
					
					PORTD &= (0 << PINB7);
					
					PORTD &= (0 << PIND6);
					PORTD &= (0 << PIND5);
					PORTB |= (1 << PINB6);
					//PORTB = 0xFF;
					//PORTD = 0;
					break;
					
				}
				//right
				case(-2): {
					
					PORTD &= (0 << PIND5);
					
					PORTD &= (0 << PINB7);
					PORTB &= (0 << PINB0);
					PORTD |= (1 << PIND6);
					break;
				}
				//left
				case(2): {
					
					PORTD &= (0 << PIND6);
					
					PORTB &= (0 << PINB6);
					PORTB &= (0 << PINB7);
					PORTD |= (1 << PIND5);
					break;
				}
				//none. turn all off
				default: {
					PORTD &= (0 << PIND5);
					PORTD &= (0 << PIND6);
					PORTD &= (0 << PINB0);
					PORTB &= (0 << PINB7);
					PORTB = 0xFF;
					PORTD = 0xFF;
					break;
				}
			}
		}
		
		_delay_ms(1000);
		/*
		x = getX();
		y = getY();
		z = getZ();
		
		if(x > y)
		{
			PORTD &= (0 << PIND6);
			
			PORTB &= (0 << PINB0);
			PORTD &= (0 << PINB7);
			PORTD = (1 << PIND5);
		}
		
		else {
			PORTD &= (0 << PIND6);
			
			PORTD &= (0 << PINB5);
			PORTD &= (0 << PINB7);
			PORTB = (1 << PIND0);
		}
		*/
		_delay_ms(10000);
	}

	return 0;
	
}