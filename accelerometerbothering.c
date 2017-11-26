/*
 * GccApplication14.c
 *
 * Created: 11/24/2017 5:49:10 PM
 * Author : billy
 */ 

/*
 * GccApplication13.c
 *
 * Created: 11/23/2017 1:00:23 AM
 * Author : billy
 */ 

#include<avr/io.h>
#include<time.h>
#include<math.h>
#include<util/delay.h>

static int offsets [] = {0, 0, 0};		//global offsets
static int _average = 10;				//number of iterations to find averages


//reference voltage for device
#define VOLTAGE 3.3

//ADC channels for accelerometer input
#define X_CHANNEL 5
#define Y_CHANNEL 4
#define Z_CHANNEL 3

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

//sets offsets for accelerometer. device must be held flat during calibration

void calibrate() {
  
  double var = 5000;
  double sumX = 0;
  double sumY = 0;
  double sumZ = 0;
  double x;
  double y;
  double z;
  int make_direction;
  offsets[0] = 0;
  offsets[1] = 0;
  offsets[2] = 0;
  for (int i = 0;i<var;i++)
  {
    sumX = sumX + getXVolt();
    sumY = sumY + getYVolt();
    sumZ = sumZ + getZVolt();
    // print wait indicator
	/*if (i%100 == 0)
    {
      Serial.print(".");
    }*/
  }
    //gs high
    //setOffSets(1672 - sumX / var,1671 - sumY / var, + 1876 - sumZ / var);
    //gs low
    setOffSets(1650 - sumX / var,1650 - sumY / var, + 2450 - sumZ / var);

    x = getXVolt();
    y = getYVolt();
    z = getZVolt();
    
    
  /*  if (abs(getOrientation())!=3)
  {
    Serial.print("\nunable to calibrate");
    //setOffSets(0,0,0);
  }
  else
  {
    Serial.print("\nDONE");
  }*/

   for(int i=0; i < 20; ++i)
   {
    x = getXVolt();
    y = getYVolt();
    z = getZVolt();
    
    /*Serial.println();
    Serial.print(sumX/var);
    Serial.print('\t');
    Serial.print(sumY/var);
    Serial.print('\t');
    Serial.print(sumZ/var);
    Serial.print('\n');

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.print(z);
    Serial.print('\n');

    delay(500);*/
    
   }

   /*while(1)
   {
      make_direction = make_orientation();
      Serial.println(make_direction);

      switch(make_direction)
      {
        case -1: {
          digitalWrite(5, HIGH);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(8, LOW);
          break;
        }
        case 1: {
          digitalWrite(6, HIGH);
          digitalWrite(5, LOW);
          digitalWrite(7, LOW);
          digitalWrite(8, LOW);
          break;
        }
        case -2: {
          digitalWrite(7, HIGH);
          digitalWrite(6, LOW);
          digitalWrite(5, LOW);
          digitalWrite(8, LOW);
          break;
        }
        case 2: {
          digitalWrite(8, HIGH);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(5, LOW);
          break;
        }

        default: {
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(8, LOW);
        }
        
      }

      

      delay(500);      
   }*/
   
   return;
}

//set up inputs and Analog-Digital converter
int init_ADC()
{
	//disable digital input for ADC0, 1, 2, 3, 4, 5
	//DIDR0|=0x3F;
	// set Vref = AVCC
	ADMUX |= (1<<REFS0);
	// set resolution to 10 bits
	ADMUX &= (0<<ADLAR);
	//set prescalar to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	//set to free running
	//ADCSRA &=  (1<<ADTS2) & (1<<ADTS1) & (1<<ADTS0);
	
	return 1;
}


//takes average of 10 measurements returns 1 or -1 if abs(x) is smallest, 2 or -2 if abs(y) is smallest, 3 or -3 if abs(z) is smallest
int getOrientation()
{
  int gemiddelde = 10;
  int x = 0;
  int y = 0;
  int z = 0;
  int xAbs = 0;
  int yAbs = 0;
  int zAbs = 0;
  for(int i = 0; i<gemiddelde ; i++)              //We take in this case 10 measurements to average the error a little bit
  {
    x = x+getXAccel();
    y = y+getYAccel();
    z = z+getZAccel();
  }
  x= x/gemiddelde;
  y = y/gemiddelde;
  z = z/gemiddelde;
  //xAbs = abs(100-abs(x));
  //yAbs = abs(100-abs(y));
  //zAbs = abs(100-abs(z));

  /*Serial.print(x);
  Serial.print('\t');
  Serial.print(y);*/

  xAbs = abs(x);
  yAbs = abs(y);
  zAbs = abs(z);
  if (xAbs<yAbs&&xAbs<zAbs)
  {
    if (x>0)
    {
      return 1;
    }
    return -1;
  }
  if (yAbs<xAbs&&yAbs<zAbs)
  {
    if (y>0)
    {
      return 2;
    }
    return -2;
  }
  if (zAbs<xAbs&&zAbs<yAbs)
  {
    if (z>0)
    {
      return 3;
    }
    return -3;
  }
  return 0;
}

//takes _average number of x measurements mapped to G range
int getXAccel()
{
  int sum = 0;
  for (int i = 0;i<_average;i++)
  {
    sum = sum + _mapMMA7361G(getXRaw());
  }
  return sum/_average;
}

//takes _average number of y measurements mapped to G range
int getYAccel()
{
  int sum = 0;
  for (int i = 0;i<_average;i++)
  {
    sum = sum + _mapMMA7361G(getYRaw());
  }
  return sum/_average;
}

//takes _average number of y measurements mapped to G range
int getZAccel()
{
  int sum = 0;
  for (int i = 0;i<_average;i++)
  {
    sum = sum + _mapMMA7361G(getZRaw());
  }
  return sum/_average;
}

//map to voltage scale (0 = 0V,  3300 = 3.3 V)
int _mapMMA7361V(int value)
{
  
    return ATmap(value,0,1024,0,3300);
  
}

//raw accelerometer reading. adds offsets calculated in calibration
int getXRaw()
{
	int x = 0;
	//set ADC channel to x channel
	ADMUX &= 0xE0;
	ADMUX |= X_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	
	
	ADCSRA|=(1<<ADIF);
	//read ADC
	x |= ADCL;
	x |= (ADCH << 8);
	_delay_us(1000);
	return x + offsets[0]+2;
}


//raw accelerometer reading. adds offsets calculated in calibration
int getYRaw()
{
	int y = 0;
	//set ADC channel to x channel
	ADMUX &= 0xE0;
	ADMUX |= Y_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	//read ADC
	y |= ADCL;
	y |= (ADCH << 8);
	
	_delay_us(1000);
	return y + offsets[1]+2;
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
	z |= ADCL;
	z |= (ADCH << 8);
	
	
	_delay_us(1000);
	
	return z + offsets[2];
}


//set raw values in scale of voltage
int getXVolt()
{
  return _mapMMA7361V(getXRaw());
}

//set raw values in scale of voltage
int getYVolt()
{
  return _mapMMA7361V(getYRaw());
}

//set raw values in scale of voltage
int getZVolt()
{
  return _mapMMA7361V(getZRaw());
}

//sets offsets[] array to center raw values
void setOffSets(int xOffSet, int yOffSet, int zOffSet)
{
    offsets[0]= ATmap(xOffSet,0,5000,0,1024);
    offsets[1]= ATmap(yOffSet,0,5000,0,1024);
    offsets[2]= ATmap(zOffSet,0,5000,0,1024);
}

//maps to a g scale
int _mapMMA7361G(int value)
{
  return ATmap(value,0,1024,-260,419);
}

//set threshold of ~10% from center
int make_xy(int to_make)
{
  if (to_make < 1450) //if to_make less than center - 200, return distance from center 
    return (to_make - 1650);

  if(to_make > 1850) // if to_make more than center + 200, return distance from center
    return (to_make - 1650);

  return 0;	//else return 0

    
}

//get average orientation from 10 readings then put through
//threshold of 10% from center
int make_orientation()
{
  int i;
  int gemiddelde = 10;
  int x = 0;
  int y = 0;
  int z = 0;
  int xAbs = 0;
  int yAbs = 0;
  int zAbs = 0;
  for(i = 0; i<gemiddelde ; i++)              //We take in this case 10 measurements to average the error a little bit
  {
    x = x+getXVolt();
    y = y+getYVolt();
    z = z+getZVolt();
  }
  x= x/gemiddelde;
  y = y/gemiddelde;
  z = z/gemiddelde;

  /*Serial.print(x);
  Serial.print('\t');
  Serial.println(y);*/

  x= make_xy(x);
  y = make_xy(y);
  z = make_xy(z);
  //xAbs = abs(100-abs(x));
  //yAbs = abs(100-abs(y));
  //zAbs = abs(100-abs(z));

  xAbs = abs(x);
  yAbs = abs(y);
  zAbs = abs(z);

  /*Serial.print(xAbs);
  Serial.print('\t');
  Serial.println(yAbs);
  Serial.print(x);
  Serial.print('\t');
  Serial.println(y);
  */
  
  if(x>0)
	return 1;
	
	if (x == 0)
	return 0;
return -1;
  if (xAbs > yAbs)
  {
    if (x>0)
    {
      return 1;
    }
    return -1;
  }
  //if (yAbs > xAbs)
  //{
   if (y>0)
    {
      return 2;
    }
   else return -2;
  
  
  return 0;
  
  /*if (zAbs<xAbs&&zAbs<yAbs)
  {
    if (z>0)
    {
      return 3;
    }
    return -3;
  }*/
  //return 0;
}

int getX()
{
	int x = 0;
	//set ADC channel to z channel
	ADMUX &= 0xE0;
	ADMUX |= X_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	//read ADC
	x |= ADCL;
	x |= (ADCH << 8);
	
	
	
	
	return x;
}

int getY()
{
	int y = 0;
	//set ADC channel to z channel
	ADMUX &= 0xE0;
	ADMUX |= Y_CHANNEL;
	
	//start conversion
	ADCSRA |= (1<<ADSC);
	
	//poll for done
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	//read ADC
	y |= ADCL;
	y |= (ADCH << 8);
	
	
	
	
	return y;
}

int getZ()
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
	z |= ADCL;
	z |= (ADCH << 8);
	
	
	
	
	return z;
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
	
	//do a calibrate
	calibrate();
	
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
			
			//PORTD = 0xFF;
			//PORTB = 0xFF;
			
			_delay_ms(1000);
			
			PORTB &= (0 << PINB0);
			
			PORTD &= (0 << PIND6);
			PORTD &= (0 << PIND5);
			PORTD |= (1 << PINB7);
			
			
			//PORTD = 0x0;
			//PORTB = 0x0;
			
			_delay_ms(1000);
			
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
					PORTD |= (1 << PINB7);
					//PORTD = 0xFF;
					//PORTB = 0;
					break;
				}
				//down
				case(1): {
					
					PORTD &= (0 << PINB7);
					
					PORTD &= (0 << PIND6);
					PORTD &= (0 << PIND5);
					PORTB |= (1 << PINB0);
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
					
					PORTB &= (0 << PINB0);
					PORTD &= (0 << PINB7);
					PORTD |= (1 << PIND5);
					break;
				}
				//none. turn all off
				default: {
					PORTD &= (0 << PIND5);
					PORTD &= (0 << PIND6);
					PORTD &= (0 << PINB0);
					PORTB &= (0 << PINB7);
					break;
				}
			}
		}
		
		_delay_ms(10000);
		
		x = getX();
		y = getY();
		z = getZ();
		
		if(x > y)
		{
			PORTD &= (0 << PIND6);
			
			PORTB &= (0 << PINB0);
			PORTD &= (0 << PINB7);
			PORTD |= (1 << PIND5);
		}
		
		else {
			PORTD &= (0 << PIND6);
			
			PORTD &= (0 << PINB5);
			PORTD &= (0 << PINB7);
			PORTB |= (1 << PIND0);
		}
		
		_delay_ms(10000);
	}

	return 0;
	
}