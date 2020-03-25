#include <WiFi.h>
#include <WiFiUdp.h>

//define port
const int potentiometer1 = 33;
const int potentiometer2 = 32;
const int potentiometer_weapon = 39;

//define the initial bias for potentiometer1 and 2
const int bias1 = 1888;
const int bias2 = 1800;
const int bias3 = 1905;

//WiFi information
const char* ssid = "GMHotel";
const char* pass = "123456789";

WiFiServer server(80);
WiFiUDP udp;

//IP Addresses
IPAddress IPlocal(192,168,1,191);//initial local IP
IPAddress IPtarget(192,168,1,126);//initial target IP
//port
unsigned int UDPlocalPort = 2080; //UDP port number for local ESP
unsigned int UDPtargetPort = 2080;//UDP port number for target ESP
//packet information
const int packetSize = 40;
char sendBuffer[packetSize+1];
byte receiveBuffer[packetSize+1];

//define variables

void setup() {
  Serial.begin(115200);
  //WiFi Setup
  WiFi.config(IPlocal, IPAddress(192, 168, 1, 1),IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid,pass);
  udp.begin(UDPlocalPort); 
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  //Set pin mode
  pinMode(potentiometer1,INPUT);
  pinMode(potentiometer2,INPUT);
  pinMode(potentiometer_weapon,INPUT);
}

void loop() {
  int motor_speed = analogRead(potentiometer1);
  int motor_diff = analogRead(potentiometer2);
  int weapon_speed = analogRead(potentiometer_weapon); 
  
  int motor_speed_new = 2048;
  int motor_diff_new = 2048;
  int weapon_new = 2048;
  Serial.print("potentiometer1  ");
  Serial.println(motor_speed);
  Serial.print("potentiometer2  ");
  Serial.println(motor_diff);
  Serial.print("potentiometer_weapon  ");
  Serial.println(weapon_speed);
  if(motor_speed>bias1)
  {
     motor_speed_new = map(motor_speed,bias1+1,4095,2048,4095); 
  }
  else
  {
     motor_speed_new = map(motor_speed,0,bias1,0,2047); 
  }
  
  if(motor_diff>bias2)
  {
     motor_diff_new = map(motor_diff,bias2+1,4095,2048,4095); 
  }
  else
  {
     motor_diff_new = map(motor_diff,0,bias2,0,2047); 
  }
  
  if(weapon_speed>bias3)
  {
     weapon_new = map(weapon_speed,bias3+1,4095,2048,4095); 
  }
  else
  {
     weapon_new = map(weapon_speed,0,bias3,0,2047); 
  } 
  
  UDPsendData(motor_speed_new,motor_diff_new,weapon_new);
  delay(10);
}

void UDPsendData(int number1,int number2,int number3){
//  Serial.println("Sending data");
  //avoid zero in the sendBuffer[1] for the first segment of the potentiometer1
  if(number1<=256)
  {
    sendBuffer[0] = 0x01;
    sendBuffer[1] = 0x01;
  }
  else
  {
    sendBuffer[0] = number1 & 0xff;
    sendBuffer[1] = number1 >> 8;
  }
  //avoid zero in the sendBuffer[3] for the frist segment of the potentiometer2
  if(number2<=256)
  {
    sendBuffer[2] = 0x01;
    sendBuffer[3] = 0x01;
  }
  else
  {
    sendBuffer[2] = number2 & 0xff;
    sendBuffer[3] = number2 >> 8;      
  }
  //avoid zero in the sendBuffer[5] for the frist segment of the potentiometer3
  if(number3<=256)
  {
    sendBuffer[4] = 0x01;
    sendBuffer[5] = 0x01;
  }
  else
  {
    sendBuffer[4] = number3 & 0xff;
    sendBuffer[5] = number3 >> 8;      
  }
  sendBuffer[6] = 0;
  udp.beginPacket(IPtarget, UDPtargetPort);
  udp.printf("%s",sendBuffer);
  udp.endPacket();
//  Serial.print("potentiometer1  ");
//  Serial.println(number1);
//  Serial.print("potentiometer2  ");
//  Serial.println(number2);
//  Serial.print("weapon  ");
//  Serial.println(number3);
}
