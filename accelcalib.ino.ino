//global variables instead of class members
int offsets[] = {0, 0, 0};
int _average = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT);
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(getOrientation());
  Serial.print("\nCalibrating MMA7361011");
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
    if (i%100 == 0)
    {
      Serial.print(".");
    }
  }
    //gs high
    //setOffSets(1672 - sumX / var,1671 - sumY / var, + 1876 - sumZ / var);
    //gs low
    setOffSets(1650 - sumX / var,1650 - sumY / var, + 2450 - sumZ / var);

    x = getXVolt();
    y = getYVolt();
    z = getZVolt();
    
    Serial.println();
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

    Serial.println(abs(getOrientation()));
    if (abs(getOrientation())!=3)
  {
    Serial.print("\nunable to calibrate");
    //setOffSets(0,0,0);
  }
  else
  {
    Serial.print("\nDONE");
  }

   for(int i=0; i < 20; ++i)
   {
    x = getXVolt();
    y = getYVolt();
    z = getZVolt();
    
    Serial.println();
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

    delay(500);
    
   }

   while(1)
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
   }
  
}

//takes average of 10 measurements
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

  Serial.print(x);
  Serial.print('\t');
  Serial.print(y);

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

//
int getXAccel()
{
  int sum = 0;
  for (int i = 0;i<_average;i++)
  {
    sum = sum + _mapMMA7361G(getXRaw());
  }
  return sum/_average;
}

int getYAccel()
{
  int sum = 0;
  for (int i = 0;i<_average;i++)
  {
    sum = sum + _mapMMA7361G(getYRaw());
  }
  return sum/_average;
}

//get average and place on g scale.
int getZAccel()
{
  int sum = 0;
  for (int i = 0;i<_average;i++)
  {
    sum = sum + _mapMMA7361G(getZRaw());
  }
  return sum/_average;
}

//map to voltage
int _mapMMA7361V(int value)
{
  
    return map(value,0,1024,0,5000);
  
}

//raw accelerometer reading. adds offsets calculated in calibration
int getXRaw()
{
  return analogRead(5) + offsets[0] + 2;
}

int getYRaw()
{
  return analogRead(4) + offsets[1] + 2;
}

int getZRaw()
{
  return analogRead(3) + offsets[2];
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

void setOffSets(int xOffSet, int yOffSet, int zOffSet)
{
    offsets[0]= map(xOffSet,0,5000,0,1024);
    offsets[1]= map(yOffSet,0,5000,0,1024);
    offsets[2]= map(zOffSet,0,5000,0,1024);
}

int _mapMMA7361G(int value)
{
  return map(value,0,1024,-260,419);
}

//set threshold of ~10% from center
int make_xy(int to_make)
{
  if (to_make < 1500)
    return (to_make - 1500);

  if(to_make > 1800)
    return (to_make - 1800);

   return 0;

    
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

  Serial.print(x);
  Serial.print('\t');
  Serial.println(y);

  x= make_xy(x);
  y = make_xy(y);
  z = make_xy(z);
  //xAbs = abs(100-abs(x));
  //yAbs = abs(100-abs(y));
  //zAbs = abs(100-abs(z));

  xAbs = abs(x);
  yAbs = abs(y);
  zAbs = abs(z);

  Serial.print(xAbs);
  Serial.print('\t');
  Serial.println(yAbs);
  Serial.print(x);
  Serial.print('\t');
  Serial.println(y);
  
  if (xAbs > yAbs)
  {
    if (x>0)
    {
      return 1;
    }
    return -1;
  }
  if (yAbs > xAbs)
  {
    if (y>0)
    {
      return 2;
    }
    return -2;
  }
  /*if (zAbs<xAbs&&zAbs<yAbs)
  {
    if (z>0)
    {
      return 3;
    }
    return -3;
  }*/
  return 0;
}
