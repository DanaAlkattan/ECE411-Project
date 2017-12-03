/*************************************************************************
 *************************************************************************
							LED MATRIX MAZE GAME 
 * Authors: Thao Tran
			Dana Alkattan
			William Boyd
			Andrew Vo
 *************************************************************************
 ************************************************************************/

#define	  F_CPU			16000000 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit)))

//reference voltage for device
#define VOLTAGE 3.3

//ADC channels for accelerometer input
#define X_CHANNEL 0x40  // pin 23 - pc0
#define Y_CHANNEL 0x41	// pin 24 - pc1
#define Z_CHANNEL 0x42	// pin 25 - pc2

int currRow = 0;
int currCol = 0;

int cursor_counter = 0;
int exit_counter = 0;

int make_orientation();
int getXRaw();
int getYRaw();
int getZRaw();
int init_ADC();

/*************************************************************************
						Greeting & 10 Mazes
*************************************************************************/
/*uint16_t TEMP[8] = {0b1000000011100111, // for testing
					0b0100000011111111,
					0b0010000011111111,
					0b0001000001111110,
					0b0000100011111111,
					0b0000010011111111,
					0b0000001011111111,
					0b0000000111111111};
*/				
uint16_t SMILE[8] = {	0b0111111111000011,
						0b1011111110000001,
						0b1101111100100100,
						0b1110111100000000,
						0b1111011100100100,
						0b1111101100011000,
						0b1111110110000001,
						0b1111111011000011};	
							
uint16_t HELLO[8] = {	0b0111111100000000,
						0b1011111101010010,
						0b1101111101010010,
						0b1110111101011110,
						0b1111011101010010,
						0b1111101101010010,
						0b1111110100000000,
						0b1111111000000000};


uint16_t THREE[8] = {	0b0111111100000000,
						0b1011111100111100,
						0b1101111100100000,
						0b1110111100111100,
						0b1111011100100000,
						0b1111101100111100,
						0b1111110100000000,
						0b1111111000000000};

uint16_t TWO[8]	  = {	0b0111111100000000,
						0b1011111100111100,
						0b1101111100100000,
						0b1110111100111100,
						0b1111011100000100,
						0b1111101100111100,
						0b1111110100000000,
						0b1111111000000000};


uint16_t ONE[8]	  = {	0b0111111100000000,
						0b1011111100010000,
						0b1101111100011000,
						0b1110111100010000,
						0b1111011100010000,
						0b1111101100010000,
						0b1111110100000000,
						0b1111111000000000};

 
uint16_t CLEAR[8] = {	0b0111111100000000,
						0b1011111100000000,
						0b1101111100000000,
						0b1110111100000000,
						0b1111011100000000,
						0b1111101100000000,
						0b1111110100000000,
						0b1111111000000000};
						
uint16_t MAZE1[8] =	{	0b0111111111111110,
						0b1011111110000000,
						0b1101111110100111,
						0b1110111110100001,
						0b1111011110111101,
						0b1111101110000001,
						0b1111110111111101,
						0b1111111000000001};



uint16_t MAZE2[8] = {	0b0111111101000110,
						0b1011111101000100,
						0b1101111111101101,
						0b1110111100000001,
						0b1111011101110111,
						0b1111101100010100,
						0b1111110101000110,
						0b1111111001110000};




uint16_t MAZE3[8] = {	0b0111111111111110,
						0b1011111110000000,
						0b1101111110111010,
						0b1110111110101010,
						0b1111011110101010,
						0b1111101110101010,
						0b1111110100001110,
						0b1111111001100000};



uint16_t MAZE4[8] = {	0b0111111100011110,
						0b1011111101101000,
						0b1101111110001101,
						0b1110111101100001,
						0b1111011101001111,
						0b1111101111100001,
						0b1111110110001111,
						0b1111111000110001};




uint16_t MAZE5[8] = {	0b0111111101000110,
						0b1011111101011100,
						0b1101111110010001,
						0b1110111110010111,
						0b1111011110110000,
						0b1111101110000010,
						0b1111110101101001,
						0b1111111000001110};



uint16_t MAZE6[8] = {	0b0111111101111100,
						0b1011111110000001,
						0b1101111101101001,
						0b1110111100100111,
						0b1111011111101001,
						0b1111101110000010,
						0b1111110100101110,
						0b1111111001111000};



uint16_t MAZE7[8] = {	0b0111111111111110,
						0b1011111110000000,
						0b1101111111010101,
						0b1110111111010101,
						0b1111011111010101,
						0b1111101111010101,
						0b1111110100001101,
						0b1111111001101111};


uint16_t MAZE0[8] = {	0b0111111111111110,
						0b1011111110000000,
						0b1101111110110101,
						0b1110111100000101,
						0b1111011110100001,
						0b1111101110111101,
						0b1111110110000001,
						0b1111111000111111};


uint16_t MAZE8[8] = {	0b0111111111011110,
						0b1011111111010000,
						0b1101111110010111,
						0b1110111110010001,
						0b1111011110110101,
						0b1111101110000101,
						0b1111110100110001,
						0b1111111001111101};



uint16_t MAZE9[8] = {	0b0111111101111110,
						0b1011111101000000,
						0b1101111101011111,
						0b1110111110000001,
						0b1111011111111101,
						0b1111101110000001,
						0b1111110110111011,
						0b1111111000000001};


uint16_t MAZE10[8] = {	0b0111111101111110,
						0b1011111100010000,
						0b1101111111010111,
						0b1110111110010001,
						0b1111011111011101,
						0b1111101101000001,
						0b1111110100011101,
						0b1111111001100000};




 
/*************************************************************************
						Configure Connections
*************************************************************************/
#define		HC595_DDR		DDRD		
#define		HC595_PORT		PORTD
#define		HC595_IN		PD0		//Data input to the first register
#define		HC595_CLOCK		PD1		//Clock signal
#define		HC595_LATCH		PD2		//Latch signal
#define		HC595_CLEAR		PD5		//Clear signal

/*************************************************************************
						HC595 Initialization
*************************************************************************/
void HC595_Init(void)
{
	//Set PORTD0-2 to be outputs
	HC595_DDR |= (1<<HC595_IN) | (1<<HC595_CLOCK) | (1<<HC595_LATCH) | (1<<HC595_CLEAR);
	
	//Pull clear high
	HC595_PORT |= (1<<HC595_CLEAR);
}

/*************************************************************************
						HC595 Clock Pulse
*************************************************************************/
void HC595_Pulse(void)
{
	HC595_PORT |= (1<<HC595_CLOCK);
	//_delay_us(1);
	HC595_PORT &= ~(1<<HC595_CLOCK);
}

/*************************************************************************
							HC595 Latch
*************************************************************************/
void HC595_Latch(void)
{
	HC595_PORT |= (1<<HC595_LATCH);
	HC595_PORT &= ~(1<<HC595_LATCH);

}

/*************************************************************************
						HC595 Send Serial Inputs
*************************************************************************/
void HC595_Shift(uint16_t data)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		if (data & 0b1000000000000000)
			HC595_PORT |= (1<<HC595_IN);	//Data input = 1
		else 
			HC595_PORT &= ~(1<<HC595_IN);	//Data input = 0
		
		HC595_Pulse();	//Pulse the clock
		
		data = data << 1; 
	}
	
	HC595_Latch();		//Latch data to outputs
}
/************************************************************************
// moving the cursor based on tilting
************************************************************************/

void accelerometer_logic(uint16_t data[])
{			
	//int direction = 0;
	//direction = make_orientation();
	switch(make_orientation())
	{
		case(2): 			// tilt right
			if(currCol == 7)
			{
				break;
			}else
			{
				if((data[currRow] & (1 << (currCol + 1))) != (1 << (currCol+1)) || ((currRow == 7) && (currCol == 6)))// the right bit is = 0, turn on
				{
					data[currRow] &= ~(1 << (currCol));
					currCol++;
					break;
				}else
				break;
			}
		case(1):				// tilt left
			if(currCol == 0)
			{
				break;
			}else
			{
				if((data[currRow] & (1<< (currCol - 1))) != (1 << (currCol - 1)))// the left bit is = 0, turn on
				{
					data[currRow] &= ~(1 << currCol);
					currCol--;
					break;
				}else
				break;
			}
		case(3):				// tilt up
			if(currRow == 0)
			{
				break;
			}else
			{
				if((data[currRow-1] & (1 << currCol)) != (1 << currCol)) // the upper bit is = 0, turn on
				{
					data[currRow] &= ~(1 << currCol);
					currRow--;
					break;
				}else
				break;
			}
		case(4):				// tilt down
			if(currRow == 7)
			{
				break;
			}else
			{
				if((data[currRow+1] & (1 << currCol)) != (1 << currCol) || (currRow == 6 && currCol == 7)) // the down bit is = 0, turn on
				{
					data[currRow] &= ~(1 << currCol);
					currRow++;
					break;
				}else
				break;
			}
		default:
			break;
	}
}

/*************************************************************************
						Button Interrupt Init
*************************************************************************/
void Button_Init(void)
{
	DDRD &= ~(1 << PORTD3);		//Set PD3 for input
	//PORTD |= (1 << PORTD3);		// set PD2/INT0 (pin 4) internal pull-up resistor*/

	PCICR |= (1 << PCIE2);      //Set pin-change interrupt for D pins
	PCMSK2 |= (1 << PCINT19);   //Mask PCINT19
	sei();                      //Set global interrupt enable bit
}

/*************************************************************************
							Greeting Matrix
*************************************************************************/
void Greet_Matrix(uint16_t data[])
{
	for (int i = 0; i < 2000; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			HC595_Shift(data[j]);
		}
	}
}

/*************************************************************************
							LED Matrix
*************************************************************************/
void LED_Matrix(uint16_t data[])
{	
	//accelerometer_logic(data);
	cursor_counter++;
	exit_counter++;	
	for (int i = 0; i < 500; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			HC595_Shift(data[j]);		
			//_delay_ms(1000);
		}	
		if (exit_counter == 1)
		{
			data[7] ^= (1 << 7);
			exit_counter = 0;
		}
		if (cursor_counter == 2)
		{
			accelerometer_logic(data);
			data[currRow] ^= (1 << currCol);
			cursor_counter = 0;
		}
	}		
}



/*************************************************************************
							ISR Service
*************************************************************************/
ISR(PCINT2_vect)
{   
	// Get a random number (0 to 10)
	
	int randNumber = 0;	
	randNumber = rand() % 11; 								
	if (BIT_IS_CLEAR(PIND, PD3))
	{	
		Greet_Matrix(THREE);
		Greet_Matrix(TWO);
		Greet_Matrix(ONE);
		
		while(1)
		{
			if(currRow == 7 && currCol == 7)
			{
				for (int j = 0; j < 3; ++j)
				{
					currRow = 0;
					currCol = 0;
					Greet_Matrix(SMILE);
				}
				return;
			}
			switch (randNumber)
			{
				case(0):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE0);
						break;
					}
				case(1):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE1);
						break;
					}
				case(2):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE2);
						break;
					}
				case(3):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE3);
						break;
					}
				case(4):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE4);
						break;
					}
				case(5):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE5);
						break;
					}
				case(6):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE6);
						break;
					}
				case(7):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE7);
						break;
					}
				case(8):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE8);
						break;
					}
				case(9):
					{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE9);
						break;
					}
				case(10):
				{
						//LED_Matrix(TEMP);
						LED_Matrix(MAZE10);
						break;
				}
			}
		}
	}
	
	Greet_Matrix(CLEAR);
}

//set up inputs and Analog-Digital converter
int init_ADC()
{
	//disable digital input for ADC0, 1, 2, 3, 4, 5
	//DIDR0|=0x3F;
	// set Vref = AVCC
	//ADMUX = 0x60;
	DDRC |= (1<<PORTC3) | (1<<PORTC4) | (1<<PORTC5);   
	PORTC |= (1<<PORTC3);
	PORTC &= ~(1<<PORTC4);
	PORTC &= ~(1<<PORTC5);
	
	DDRD |= (1<<PORTD4);
	PORTD &= ~(1<<PORTD4);
	
	ADMUX &= 0b01111111;
	ADMUX |= (1<<REFS0);
	//ADMUX |= (1<<ADLAR);
	// set resolution to 10 bits
	//ADMUX &= (0<<ADLAR);
	//set prescalar to 16 and enable ADC
	ADCSRA = 0x80;
	ADCSRA |= (1<<ADPS2)|(1<<ADEN);
	//ADCSRA &= ~(1<<ADPS1) & ~(1<<ADPS0);
	//set to free running
	ADCSRB &=  0xF8;
	//ADMUX |= (1 << REFS0) | (1<< REFS1);
	
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
	//_delay_us(100);
	return (high * 256) + low; //+ offsets[0]+2;
	//return ADC;
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
	
	//_delay_us(100);
	return (high * 256) + low;
	//return ADC;
}

//raw accelerometer reading. adds offsets calculated in calibration
/*int getZRaw()
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
	
	
	//_delay_us(100);
	
	//return z;
	return ADC;
}
*/

int make_orientation()
{
	int gemiddelde = 10;
	int x = 0;
	int y = 0;
	int xAbs;
	int yAbs;
//	int z = 0;
	for(int i = 0; i<gemiddelde ; i++)              //We take in this case 10 measurements to average the error a little bit
	{
		x = x+getXRaw();
		y = y+getYRaw();
//		z = z+getZRaw();
	}
	x= x/gemiddelde;
	y = y/gemiddelde;
//	z = z/gemiddelde;
	xAbs = abs(x -512);
	yAbs = abs(y -512);
	if (xAbs> yAbs)
	{
		if (x < 400)
		{
			x = 0;
			y = 0;
			return 3;  // up
		}
		else if(x>624)	
		{
			x = 0;
			y = 0;
			return 4;	// down
		}
		else
		{
			x = 0;
			y = 0;
			return 0;
		}
	}
	else
	{	
		 if (y < 400)
		{
			x = 0;
			y = 0;
			return 1;	// right
		}
		else if(y > 624)
		{
			x = 0;
			y = 0;
			return 2;		// left
		}
		else
		{
			x = 0;
			y = 0;
			return 0;
		}
	}
}

int main(void)
{							
	Button_Init();
	HC595_Init();	
	init_ADC();
	
	while(1)
	{		
		//HC595_Shift(0000000101111111);
		Greet_Matrix(HELLO);
		//_delay_ms(1000);
	}
}
