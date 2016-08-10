#define TRIG_PIN 12  //ultra
#define ECHO_PIN 11  //ultra
#define lDR_PIN A1
#define HUMID_PIN A0
#define LED_PIN 13
#define TEMP_PIN A2
#define SERVO_PIN 3
#//define MOTION 10

#include <Servo.h>
Servo myservo;

#include <LiquidCrystal.h> //liquidcrystal
LiquidCrystal lcd(8,9,4,5,6,7);

#include <pt.h>
#define PT_DELAY(pt, ms, ts)\
  ts = millis();\
  PT_WAIT_WHILE(pt, millis()-ts < (ms));

struct pt pt_taskVol;
struct pt pt_tasksoilmoist;
struct pt pt_taskLdr;
struct pt pt_tasktemp;
struct pt pt_taskSend;
struct pt pt_taskServo;

float in = 0;
int vol = 0;
String str ="";
int persensor;
int sensorValue = 0;
int analight;
int percentlight;
int temp;

String waterSign="";
double vold=0;
double vwater=0;
double h=0;
double ha=0;
double avr = 70;
int delh;
int tang=0;
int delvol=0;
String usedwater="";

String s_temp="";
String s_humid="";
String s_vol="";
String s_ldr="" ;

PT_THREAD(taskVol(struct pt*pt)){
  //in = alert();
  static uint32_t ts;
  
  PT_BEGIN(pt);

  while(1){
    long duration, distanceCm, distanceIn;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN,HIGH);
    //Serial.print( "duration: " );
    //Serial.println( duration );
  
    in = duration / 29.1 / 2 ;
    //tang = (22/7)*4.21*4.21*(9-in);
    //Serial.print("tang: ");
    //Serial.println(tang);
    //Serial.println(in);
    vol = ((9-in)*26.5*26.5)/(4*(22.0/7));
    if(vol < 0)
      vol=0;
    //Serial.print("vol: ");
    //Serial.println(vol);
    //str = str + vol + "-";
    s_vol = String(vol);
    PT_DELAY(pt,100,ts);
  }
  
  PT_END(pt); 
}

PT_THREAD(taskServo(struct pt*pt)){
  static uint32_t ts;

  PT_BEGIN(pt);
  
  while(1){
    if(Serial1.available()){
      waterSign = Serial1.readStringUntil('\r');
      waterSign.replace("\r","");
      waterSign.replace("\n","");
      Serial.println(waterSign);
      if(waterSign == "1"){
        if(persensor < 100){//for true 70
          vold = (persensor*1070)/100;  //vold
          vwater = (7490/3) - ((10/3)*vold);
          delh = (4*(22.0/7)*vwater)/(26.5*26.5);
          //permoist = ( ( vol+((26*(delh))/(4*(22.0/7))) ) / (1070+((26*(delh))/(4*(22.0/7))) ) )*100.0;
          h = (4*(22.0/7)*vol)/(26.5*26.5);
          h = h - delh;
          myservo.write(20); //open
          Serial.println("owner : 1");
          while(ha>h){
            long duration, distanceCm, distanceIn;
            digitalWrite(TRIG_PIN, LOW);
            delayMicroseconds(2);
            digitalWrite(TRIG_PIN, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG_PIN, LOW);
            duration = pulseIn(ECHO_PIN,HIGH);
            //Serial.print( "duration: " );
            //Serial.println( duration );
  
            in = duration / 29.1 / 2 ;
            ha = 9-in;
            Serial.print("ha: ");
            Serial.print(ha);
            //myservo.write(30); //open
          }
          delvol = delh*((26.5*26.5*7)/(4*22));
          //delay(2000);//
          usedwater = "_"+String(delvol);
          Serial1.println(usedwater);
          myservo.write(150); //close
          Serial.println("owmer : 2");
        }else{
          Serial1.println("You should not to fill water.");
        }
      }else if(waterSign == "2"){
          tang = 450-vol;
          Serial1.println(tang);
          Serial.print("tang : ");
          Serial.println(tang);
      }
        
    }
    PT_DELAY(pt,100,ts);
  }
  
  PT_END(pt);  
}

PT_THREAD(tasksoilmoist(struct pt* pt)){
  static uint32_t ts;
  
  PT_BEGIN(pt);
  
  while(1){
    sensorValue = analogRead(HUMID_PIN); //humid soil //--sendData  
    digitalWrite(LED_PIN, HIGH);  
    PT_DELAY(pt,sensorValue,ts);             
    digitalWrite(LED_PIN, LOW);   
    PT_DELAY(pt,sensorValue,ts);
    persensor = 100-(sensorValue*100)/1023;
    //Serial.print("Moisture of Soil (%) :  ");
    //str += persensor;
    //str += "-";
    //Serial1.println(persensor);
    //Serial.print("moist: ");
    //Serial.println(persensor);
    s_humid = String(persensor);
    PT_DELAY(pt,100,ts);
  }

  PT_END(pt);
}

PT_THREAD(tasktemp(struct pt* pt)){
  static uint32_t ts;
  
  PT_BEGIN(pt);

  while(1){
    temp = (25*analogRead(TEMP_PIN) - 2050)/100; //temp  //--sendData
    //str += temp;         
    //Serial.print("temp: ");
    //Serial.println(temp);
    s_temp = String(temp);
    PT_DELAY(pt,100,ts);
  }

  PT_END(pt);
}

PT_THREAD(taskLdr(struct pt*pt)){
  static uint32_t ts;

  PT_BEGIN(pt);
  
  while(1){
    analight = analogRead(lDR_PIN); //ldr   //--sendData
    percentlight = analight*100/1023;
    //str += percentlight;           
    //str += "-";
    //Serial.print("ldr: ");
    //Serial.println(percentlight);
    s_ldr = String(percentlight);
    PT_DELAY(pt,100,ts);
  }
  
  PT_END(pt);  
}

  

PT_THREAD(taskSend(struct pt*pt)){
  static uint32_t ts;

  PT_BEGIN(pt);

  while(1){
    /*Serial.println(str+"test");
    //Serial1.println("b");
    Serial1.println(str);
    //Serial1.println("1l2");
    str = "";
    PT_DELAY(pt,1000,ts);*/
    str = s_vol +"-"+ s_humid +"-"+ s_ldr +"-"+ s_temp;
    //Serial.println(str);
    Serial1.println(str);
    str="";
    PT_DELAY(pt,500,ts);
  }

  PT_END(pt);
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  PT_INIT(&pt_taskVol);
  PT_INIT(&pt_tasksoilmoist);
  PT_INIT(&pt_taskLdr);
  PT_INIT(&pt_tasktemp);
  PT_INIT(&pt_taskSend);
  PT_INIT(&pt_taskServo);
  
  pinMode(LED_PIN, OUTPUT); //humid soil 
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT); 
  pinMode(lDR_PIN, INPUT); //ldr
  pinMode(TEMP_PIN, INPUT); //temp 
  //pinMode(MOTION, INPUT); 
  
  myservo.attach(SERVO_PIN); 
  
  lcd.begin(16,2); //liquidcrystal
  lcd.setCursor(4,0); //liquidcrystal
  lcd.print("EXTERNAL");
  lcd.setCursor(0,1);
  lcd.print("FUTURE EASY LIFE");  
}

void loop() {
  taskVol(&pt_taskVol);
  tasksoilmoist(&pt_tasksoilmoist);
  taskLdr(&pt_taskLdr);
  tasktemp(&pt_tasktemp);
  taskSend(&pt_taskSend);
  taskServo(&pt_taskServo);
  //int motion = digitalRead(MOTION);
  //Serial.println(motion);
}
