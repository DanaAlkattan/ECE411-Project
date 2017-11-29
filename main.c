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
#define X_CHANNEL 0x45
#define Y_CHANNEL 0x44
#define Z_CHANNEL 0x43

//int direction = 0;
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
uint16_t TEMP[8] = {0b1000000011100111, // for testing
					0b0100000011111111,
					0b0010000011111111,
					0b0001000001111110,
					0b0000100011111111,
					0b0000010011111111,
					0b0000001011111111,
					0b0000000111111111};
					
uint16_t SMILE[8] = {	0b1000000000111100,
						0b0100000001111110,
						0b0010000011011011,
						0b0001000011111111,
						0b0000100011011011,
						0b0000010011100111,
						0b0000001001111110,
						0b0000000100111100};	
							
uint16_t HELLO[8] = {	0b1000000011111111,
						0b0100000010101101,
						0b0010000010101101,
						0b0001000010100001,
						0b0000100010101101,
						0b0000010010101101,
						0b0000001011111111,
						0b0000000111111111};


uint16_t THREE[8] = {	0b1000000011111111,
						0b0100000011000011,
						0b0010000011011111,
						0b0001000011000011,
						0b0000100011011111,
						0b0000010011000011,
						0b0000001011111111,
						0b0000000111111111};

uint16_t TWO[8]	  = {	0b1000000011111111,
						0b0100000011000011,
						0b0010000011011111,
						0b0001000011000011,
						0b0000100011111011,
						0b0000010011000011,
						0b0000001011111111,
						0b0000000111111111};


uint16_t ONE[8]	  = {	0b1000000011111111,
						0b0100000011101111,
						0b0010000011100111,
						0b0001000011101111,
						0b0000100011101111,
						0b0000010011101111,
						0b0000001011111111,
						0b0000000111111111};

 
uint16_t CLEAR[8] = {	0b1000000011111111,
						0b0100000011111111,
						0b0010000011111111,
						0b0001000011111111,
						0b0000100011111111,
						0b0000010011111111,
						0b0000001011111111,
						0b0000000111111111};
					 
uint16_t MAZE1[8] =	{	0b1000000000000001,
						0b0100000001111111,
						0b0010000001011000,
						0b0001000001011110,
						0b0000100001000010,
						0b0000010001111110,
						0b0000001000000010,
						0b0000000111111110};


uint16_t MAZE2[8] = {	0b1000000010111001,
						0b0100000010111011,
						0b0010000000010010,
						0b0001000011111110,
						0b0000100010001000,
						0b0000010011101011,
						0b0000001010111001,
						0b0000000110001111};


uint16_t MAZE3[8] = {	0b1000000000000001,
						0b0100000001111111,
						0b0010000001000101,
						0b0001000001010101,
						0b0000100001010101,
						0b0000010001010101,
						0b0000001011110001,
						0b0000000111000111};

uint16_t MAZE4[8] = {	0b1000000011100001,
						0b0100000010010111,
						0b0010000001110010,
						0b0001000010011110,
						0b0000100010110000,
						0b0000010000011110,
						0b0000001001110000,
						0b0000000111001110};



uint16_t MAZE5[8] = {	0b1000000010111001,
						0b0100000010100011,
						0b0010000001101110,
						0b0001000001101000,
						0b0000100001001100,
						0b0000010001111100,
						0b0000001010100110,
						0b0000000111110001};


uint16_t MAZE6[8] = {	0b1000000010000011,
						0b0100000001111110,
						0b0010000010010110,
						0b0001000011011000,
						0b0000100000010110,
						0b0000010001111101,
						0b0000001011010001,
						0b0000000110000111};


uint16_t MAZE7[8] = {	0b1000000000000001,
						0b0100000001111111,
						0b0010000000101010,
						0b0001000000101010,
						0b0000100000101010,
						0b0000010000101010,
						0b0000001011110010,
						0b0000000110010000};

uint16_t MAZE0[8] = {	0b1000000000000001,
						0b0100000001111111,
						0b0010000001001010,
						0b0001000011111010,
						0b0000100001011110,
						0b0000010001000010,
						0b0000001001111110,
						0b0000000111000000};

uint16_t MAZE8[8] = {	0b1000000000100001,
						0b0100000000101111,
						0b0010000001101000,
						0b0001000001101110,
						0b0000100001001010,
						0b0000010001111010,
						0b0000001011001110,
						0b0000000110000010};


uint16_t MAZE9[8] = {	0b1000000010000001,
						0b0100000010111111,
						0b0010000010100000,
						0b0001000001111110,
						0b0000100000000010,
						0b0000010001111110,
						0b0000001001000100,
						0b0000000111111110};

uint16_t MAZE10[8] = {	0b1000000010000001,
						0b0100000011101111,
						0b0010000000101000,
						0b0001000001101110,
						0b0000100000100010,
						0b0000010010111110,
						0b0000001011100010,
						0b0000000111100111};



 
/*************************************************************************
						Configure Connections
*************************************************************************/
#define		HC595_DDR		DDRD		
#define		HC595_PORT		PORTD
#define		HC595_IN		PD0		//Data input to the first register
#define		HC595_CLOCK		PD1		//Clock signal
#define		HC595_LATCH		PD2		//Latch signal
#define		HC595_CLEAR		PD3		//Clear signal

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
	//returns -1 = left
	// 1 = right
	// -2 = down
	// 2 = up
	int direction = 0;
	direction = make_orientation();
	switch(direction)
	{
		case(2): 			// tilt right (down)
			if(currCol == 7)
			{
				break;
			}else
			{
				if((data[currRow] & (1 << (currCol + 1))) == (1 << (currCol+1)))// the right bit is = 0, turn on
				{
					data[currRow] |= (1 << (currCol));
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
				if((data[currRow] & (1<< (currCol - 1)))== (1 << (currCol - 1)))// the left bit is = 0, turn on
				{
					data[currRow] |= (1 << currCol);
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
				if((data[currRow-1] & (1 << currCol)) == (1 << currCol)) // the upper bit is = 0, turn on
				{
					data[currRow] |= (1 << currCol);
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
				if((data[currRow+1] & (1 << currCol)) == (1 << currCol)) // the down bit is = 0, turn on
				{
					data[currRow] |= (1 << currCol);
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
	PORTD |= (1 << PORTD3);		// set PD2/INT0 (pin 4) internal pull-up resistor*/

	PCICR |= (1 << PCIE2);      //Set pin-change interrupt for D pins
	PCMSK2 |= (1 << PCINT19);   //Mask PCINT19
	sei();                      //Set global interrupt enable bit
}

/*************************************************************************
							Greeting Matrix
*************************************************************************/
void Greet_Matrix(uint16_t data[])
{
	for (int i = 0; i < 250; i++)
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
	for (int i = 0; i < 80; i++)
	{	
		for(uint8_t j = 0; j < 8; j++)
		{
			HC595_Shift(data[j]);		
			//_delay_ms(1000);
		}
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

/*************************************************************************
							ISR Service
*************************************************************************/
ISR(PCINT2_vect)
{   
	// Get a random number (0 to 255)
	int j = 0;
	int randNumber;		
	randNumber = rand() % 10; 								
	if (BIT_IS_CLEAR(PIND, PD3))
	{	
		//Greet_Matrix(HELLO);
		Greet_Matrix(THREE);
		Greet_Matrix(TWO);
		Greet_Matrix(ONE);
		
		while(1)
		{
			if(currRow == 7 && currCol == 7)
			{
				for (j = 0; j < 2; ++j)
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

//get average orientation from 10 readings then put through
//threshold of 10% from center
//returns -1 = left
// 1 = right
// -2 = down
// 2 = up
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
		
			return 3;  // up
		}
		else if(x>624)	
		{
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
	/*********** Test Code ************
	HC595_DDR |= (1<<HC595_IN);
	
	while (1)
	{
		HC595_PORT |= (1<<HC595_IN);
		_delay_ms(3000);
		HC595_PORT &= ~(1<<HC595_IN);
		_delay_ms(3000);
	}
	***********************************/
	//entryPoint.x = 0;
	//entryPoint.y = 7;
	//exitPoint.x = 7;
	//exitPoint.y = 0;
							
	Button_Init();
	HC595_Init();	
	init_ADC();

	//int direction;
	//HC595_Shift(0b0001000011101111);
	
	//_delay_ms(100);
	
	while(1)
	{
		/*direction = make_orientation();
		
		switch (direction)
		{
			case 1: {
				HC595_Shift(0b0010000011101111);
				break;
			}
			case 2: {
				HC595_Shift(0b0000010011101111);
				break;
			}
			case 3: {
				HC595_Shift(0b0001000011011111);
				break;
			}
			case 4: {
				HC595_Shift(0b0001000011110111);
				break;
			}		
			default: {
				HC595_Shift(0b0001000011101111);
				break;
			}
		}
		_delay_ms(100);*/
		
		
		Greet_Matrix(HELLO);
		_delay_ms(1);
	}
}
