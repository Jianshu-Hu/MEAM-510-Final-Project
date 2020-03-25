#include <WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

//define port
const int left_pwm = 17;
const int right_pwm = 16;
const int left_pwm2 = 21;
const int right_pwm2 = 22;
const int weapon_pwm = 32;
//WiFi information
const char* ssid = "GMHotel";
const char* pass = "123456789";

WiFiServer server(80);
WiFiUDP udp;

//IP Addresses
IPAddress IPlocal(192,168,1,126);
IPAddress IPtarget(192,168,1,191);
//port
unsigned int UDPlocalPort = 2080;   // UDP port number for local ESP
unsigned int UDPtargetPort = 2080;  // UDP port number on the target ESP
//packet information
const int packetSize = 30;          // define packetSize (length of message)
char sendBuffer[packetSize+1];        // create the sendBuffer array
char receiveBuffer[packetSize+1];     // create the receiveBuffer array

//define constants

void setup() {
  Serial.begin(115200);

  // setup WiFi Network
  WiFi.mode(WIFI_AP);         // set the WiFi mode to be AP mode
  WiFi.softAP(ssid,pass);          // configure the ESP in AP mode with network name
  delay(100);                 // hack for AP_START...
  Serial.print("Access point ");    
    
  IPAddress gateway(192,168,1,1);                 // initialize gateway IP
  IPAddress subnet(255,255,255,0);                // initialize subnet mask
  WiFi.softAPConfig(IPlocal, gateway, subnet);    // configure the IP addresses
    
  udp.begin(UDPlocalPort);    // configure a port for UDP comms 
  server.begin();

  IPAddress myIP = WiFi.softAPIP();   
  Serial.print("AP IP address "); Serial.println(myIP);

  //pin mode setup
  pinMode(left_pwm,OUTPUT);
  pinMode(right_pwm,OUTPUT);

  //pwm output setup
  servo1.attach(left_pwm);
  servo2.attach(right_pwm);
  servo3.attach(left_pwm2);
  servo4.attach(right_pwm2);
  servo5.attach(weapon_pwm);
}

void loop() {
  UDPreceiveData();
}

void UDPreceiveData(){
  int cb = udp.parsePacket();
  if(cb)
  {
    udp.read(receiveBuffer,packetSize);
    int motor_infor1 = receiveBuffer[0] | (receiveBuffer[1] << 8);
    int motor_infor2 = receiveBuffer[2] | (receiveBuffer[3] << 8);
    rotate(motor_infor1,motor_infor2);
//    Serial.print("motor_speed");
//    Serial.println(motor_infor1);
//    Serial.print("motor_diff");
//    Serial.println(motor_infor2);    
  }
  delay(10);
}

//rotate the motor according to the message we receive
void rotate(int motor_speed,int motor_diff)
{
  //calculate the speed of left motor and right motor
  int speed_initial = 0;
  speed_initial = map(motor_speed,0,4095,0,180);
  int speed_left = 0;
  int speed_right = 0;
  //set the right speed and direction according to the speed difference
  if(speed_initial>90)
  {
    if(motor_diff<=2047)
    {
      //set the speed of right motors and decrease the speed of left motors
     
      speed_right = speed_initial;
      speed_left = map(motor_diff,0,2047,0,speed_initial);
    }
    else
    {
      //set the speed of left motors and decrease the speed of right motors
      speed_left = speed_initial;
      speed_right = map(motor_diff,2048,4095,speed_initial,0);
    }
  }
  
  if(speed_initial<90)
  {
    if(motor_diff<=2047)
    {
      //set the speed of right motors and decrease the speed of left motors
     
      speed_right = speed_initial;
      speed_left = map(motor_diff,0,2047,180,speed_initial);
    }
    else
    {
      //set the speed of left motors and decrease the speed of right motors
      speed_left = speed_initial;
      speed_right = map(motor_diff,2048,4095,speed_initial,180);
    }
  }
  
  //set the rotation speed
  servo1.write(speed_left);
  servo2.write(speed_right);
  servo3.write(speed_left);
  servo4.write(speed_right);
  servo5.write(speed_left);

//  servo1.write(90);
//  servo2.write(90);
  
  Serial.print("motor1_speed");
  Serial.println(speed_left);
  Serial.print("motor2_speed");
  Serial.println(speed_right); 
}
