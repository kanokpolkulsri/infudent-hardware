#define TRIG_PINin 8
#define ECHO_PINin 9
#include<pt.h>
struct pt pt_alert;
void setup() {
  Serial.begin(9600);
  PT_INIT(&pt_alert);
  pinMode(TRIG_PINin,OUTPUT);
  pinMode(ECHO_PINin,INPUT);
}
float alert(int TRIG_PIN,int ECHO_PIN,String s){
   long duration, distanceCm, distanceIn;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
  distanceCm = duration / 29.1 / 2 ;
  return distanceCm;
}
float in = 0;
void SerialEvent(){
  /////////////
  in = alert(TRIG_PINin,ECHO_PINin, "in");
  Serial.println(in);
  delay(100);
  
}

void loop() {
  SerialEvent();
}
