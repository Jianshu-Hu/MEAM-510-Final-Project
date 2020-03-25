#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//define the constants
const int photodiode_f = 18;
const int photodiode_b = 23;
const int left_pwm = 17;
const int right_pwm = 21;
const int left_pwm2 = 16;
const int right_pwm2 = 22;

//define variables
unsigned long low_time[7] = {0,0,0,0,0,0,0};
unsigned long x = 0;
unsigned long y = 0;
int i = 0;
//position for diode in the front
unsigned long x_f = 0;
unsigned long y_f = 0;
//position for diode at the behind 
unsigned long x_b = 0;
unsigned long y_b = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(photodiode_f,INPUT);
  pinMode(photodiode_b,INPUT);
  pinMode(left_pwm,OUTPUT);
  pinMode(right_pwm,OUTPUT);
  pinMode(left_pwm2,OUTPUT);
  pinMode(right_pwm2,OUTPUT);

  servo1.attach(left_pwm);
  servo2.attach(right_pwm);
  servo3.attach(left_pwm2);
  servo4.attach(right_pwm2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(i==0)
  {
  average1();//calculate the average x_f,y_f
  average2();//calculate the average x_b,y_b

  Serial.println("x_forward");
  Serial.println(x_f);  
  Serial.println("y_forward");
  Serial.println(y_f);  
  Serial.println("x_behand");
  Serial.println(x_b);  
  Serial.println("y_behand");
  Serial.println(y_b); 
  
  control();//implement control according to the x,y
//  Serial.println("move_forward"); 
  move_forward();
  rotate();
  move_forward2();
  i=1;
  }
}

// get the signal for the diode in the front
void capture_signal1()
{
  low_time[0] = pulseIn(photodiode_f,LOW);
  while(low_time[0] < 50)
  {
    low_time[0] = pulseIn(photodiode_f,LOW);
  }

  low_time[1] = pulseIn(photodiode_f,LOW); 
  while(low_time[1] < 50)
  {
    low_time[1] = pulseIn(photodiode_f,LOW);
  }
  
  low_time[2] = pulseIn(photodiode_f,LOW);
  while(low_time[2] < 50)
  {
    low_time[2] = pulseIn(photodiode_f,LOW);
  }
  
  low_time[3] = pulseIn(photodiode_f,LOW);
  while(low_time[3] < 50)
  {
    low_time[3] = pulseIn(photodiode_f,LOW);
  }

  low_time[4] = pulseIn(photodiode_f,LOW);
  while(low_time[4] < 50)
  {
    low_time[4] = pulseIn(photodiode_f,LOW);
  }

  low_time[5] = pulseIn(photodiode_f,LOW);
  while(low_time[5] < 50)
  {
    low_time[5] = pulseIn(photodiode_f,LOW);
  }

  low_time[6] = low_time[0];

}

// get the signal for the diode at the behind
void capture_signal2()
{
  low_time[0] = pulseIn(photodiode_b,LOW);
  while(low_time[0] < 50)
  {
    low_time[0] = pulseIn(photodiode_b,LOW);
  }

  low_time[1] = pulseIn(photodiode_b,LOW); 
  while(low_time[1] < 50)
  {
    low_time[1] = pulseIn(photodiode_b,LOW);
  }
  
  low_time[2] = pulseIn(photodiode_b,LOW);
  while(low_time[2] < 50)
  {
    low_time[2] = pulseIn(photodiode_b,LOW);
  }
  
  low_time[3] = pulseIn(photodiode_b,LOW);
  while(low_time[3] < 50)
  {
    low_time[3] = pulseIn(photodiode_b,LOW);
  }

  low_time[4] = pulseIn(photodiode_b,LOW);
  while(low_time[4] < 50)
  {
    low_time[4] = pulseIn(photodiode_b,LOW);
  }

  low_time[5] = pulseIn(photodiode_b,LOW);
  while(low_time[5] < 50)
  {
    low_time[5] = pulseIn(photodiode_b,LOW);
  }

  low_time[6] = low_time[0];

}

// average filter for x_f,y_f
void average1()
{
  unsigned long x_sum = 0;
  unsigned long y_sum = 0;
  int j = 0;
  for(j=0;j<20;j++)
  {
    int flag = 0;
    while(flag == 0)
    {
        capture_signal1();
        int i = 0;
        for(i=0;i<6;i++)
        {
          if( (low_time[i] > 7000) && (low_time[i+1] > 7000) )
          {
            break;
          }
        }
        if(i<5)
        {
          x = low_time[i+2]; 
        }
        else
        {
          x = low_time[i-4];
        }
        if(i<3)
        {
          y = low_time[i+4]; 
        }
        else
        {
          y = low_time[i-2];
        }
        if( (x<8000) && (y<8000))
        {
          flag = 1;
        }
    }
    x_sum = x_sum + x;
    y_sum = y_sum + y;
  }
    x_f = x_sum/20;
    y_f = y_sum/20;
}

// average filter for x_b,y_b
void average2()
{
  unsigned long x_sum = 0;
  unsigned long y_sum = 0;
  int j = 0;
  for(j=0;j<20;j++)
  {
    int flag = 0;
    while(flag == 0)
    {
        capture_signal2();
        int i = 0;
        for(i=0;i<6;i++)
        {
          if( (low_time[i] > 7000) && (low_time[i+1] > 7000) )
          {
            break;
          }
        }
        if(i<5)
        {
          x = low_time[i+2]; 
        }
        else
        {
          x = low_time[i-4];
        }
        if(i<3)
        {
          y = low_time[i+4]; 
        }
        else
        {
          y = low_time[i-2];
        }
        if( (x<8000) && (y<8000))
        {
          flag = 1;
        }
    }
    x_sum = x_sum + x;
    y_sum = y_sum + y;
  }
    x_b = x_sum/20;
    y_b = y_sum/20;
}

// control the car according to the position
void control()
{
  //  judge the direction
  while( (((y_f>=y_b) && (y_f-y_b>40)) ||
      ((y_f<y_b) && (y_b-y_f>40))) ||
      ((x_f<x_b) || (x_f-x_b<100))
    )
  {
    servo1.write(100);
    servo2.write(100);
    delay(200);
    average1();//calculate the average x_f,y_f
    average2();//calculate the average x_b,y_b
  }
}

void move_forward()
{
    while(x_f<4600)
    {
      servo1.write(100);
      servo2.write(80);
      servo3.write(100);
      servo4.write(80);
      delay(200);
      average1();//calculate the average x_f,y_f
      average2();//calculate the average x_b,y_b
      Serial.println("x_forward");
      Serial.println(x_f);  
      Serial.println("y_forward");
      Serial.println(y_f);  
      Serial.println("x_behand");
      Serial.println(x_b);  
      Serial.println("y_behand");
      Serial.println(y_b); 
    }
}

void rotate()
{
    servo3.detach();
    delay(100);
    servo4.detach();
    delay(100);
    while( ((x_f>=x_b) && (x_f-x_b>60)) ) 
    {
    servo1.write(100);
    servo2.write(100);
    delay(200);
    average1();//calculate the average x_f,y_f
    average2();//calculate the average x_b,y_b
    }
//    servo1.detach();
//    delay(200);
//    servo2.detach();
//    delay(200);
//    servo3.detach();
//    delay(200);
//    servo4.detach();
//    delay(200);
}

void move_forward2()
{
//    servo1.attach(left_pwm);
//    servo2.attach(right_pwm);
    servo3.attach(left_pwm2);
    servo4.attach(right_pwm2);
    while(y<2600)
    {
      servo1.write(100);
      servo2.write(80);
      servo3.write(100);
      servo4.write(80);
      delay(200);
      average1();//calculate the average x_f,y_f
      average2();//calculate the average x_b,y_b
    }
    servo1.detach();
    servo2.detach();
    servo3.detach();
    servo4.detach();
}
